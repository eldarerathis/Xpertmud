#ifdef GMCP_SUPPORT
#include "config.h"
#include "GMCP.h"
#include <arpa/telnet.h>
#include <functional>
#include <QDebug>

#ifndef TELOPT_GMCP
# define TELOPT_GMCP 201
#endif

GMCP::GMCP(Connection* parent, SenderIface* sender,
	   InputStreamFilter *first, 
	   InputStreamFilter *next):
  QObject((QObject*)parent), InputStreamFilter(first, next),
  parent(parent), sender(sender)
{
}

GMCP::~GMCP() {}

void GMCP::iacDoCallback(SenderIface * s) {
    qDebug() << "[GMCP]: Sending Core.Hello";
    s->sendByte((char)IAC);
    s->send((char[]){ (char)SB, (char)TELOPT_GMCP }, 2);
    s->send(QString(R"(Core.Hello { "client": "Xpertmud", "version": "%1" })").arg(APP_VERSION).toStdString());
    s->sendByte((char)IAC);
    s->sendByte((char)SE);

    qDebug() << "[GMCP]: Sending Core.Supports.Set";

    s->sendByte((char)IAC);
    s->send((char[]){ (char)SB, (char)TELOPT_GMCP }, 2);
    s->send(R"(Core.Supports.Set [ "Char 1", "Char.Skills 1", "Char.Items 1", "Room 1", "IRE.Rift 1" ])");
    s->sendByte((char)IAC);
    s->sendByte((char)SE);
}

void GMCP::plug(TelnetFilter& telnetFilter) {
  telnetFilter.registerIac(TELOPT_GMCP, this);
}
		 
TelnetIacAnswer GMCP::iacWont(int) {
  return TelnetIacAnswer(false);
}
TelnetIacAnswer GMCP::iacWill(int) {
  qDebug() << "[GMCP] Server sent IAC WILL GMCP";
  qDebug() << "[GMCP] -> Responding with IAC DO";
  return TelnetIacAnswer(true, true, false, &GMCP::iacDoCallback );
}
TelnetIacAnswer GMCP::iacDont(int) {
  return TelnetIacAnswer(false);
}
TelnetIacAnswer GMCP::iacDo(int) {
  return TelnetIacAnswer(true, true);
}

bool GMCP::iacSubNegotiation(int iac, const string& command) {
  QString data;
  if (iac == TELOPT_GMCP) {
    data = QString::fromStdString(command);
    parent->GMCPDataReceived(data, 0);
    return true;
  }

  return false;
}

void GMCP::parse(const string& str) {
    next->parse(str);
}
#endif