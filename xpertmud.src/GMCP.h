#ifdef GMCP_SUPPORT
#ifndef GMCP_H
#define GMCP_H

#include <QObject>
#include <vector>
#include "Connection.h"
#include "TelnetFilter.h"
#include "InputStreamHandler.h"

class GMCP: public QObject,  public TelnetIacCallback, public InputStreamFilter {
  Q_OBJECT
public:
  GMCP(Connection* parent, SenderIface* sender,
       InputStreamFilter *first = NULL, 
       InputStreamFilter *next = NULL);
  virtual ~GMCP();

  static void  iacDoCallback(SenderIface * s);
  void  plug(TelnetFilter& telnetFilter);
		 
  TelnetIacAnswer iacWont(int iac);
  TelnetIacAnswer iacWill(int iac);
  TelnetIacAnswer iacDont(int iac);
  TelnetIacAnswer iacDo(int iac);
  bool iacSubNegotiation(int iac, const string& command);

  virtual void parse(const string& str);

signals:
  void GMCPDataReceived(QString&, int);

private:
  Connection* parent;
  SenderIface* sender;
};

#endif
#endif