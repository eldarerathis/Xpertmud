// -*- c++ -*-
#ifndef XM_PYTHON_H
#define XM_PYTHON_H

#include <cmath>
// This is necessary to prevent conflicts between Qt's 'slots'
// definition and the one in Python.h 
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")
#include "../Scripting.h"


class GuiScriptingBindings;

class XMPython: public XMScripting {

Q_OBJECT

public:
  XMPython(QObject* parent,
	   const char* name);
  ~XMPython();
 
  void setCallback(GuiScriptingBindings *callBack);

  void timer();
  void textEntered(const QString & text);
  bool keyPressed(const QString & key, const QString & ascii);
  

  void textReceived(const QString & line, int id);
  void echo(bool state, int id);
  void connectionEstablished(int id);
  void connectionDropped(int id);

  void mouseDown(int id, int x, int y);
  void mouseDrag(int id, int x, int y);
  void mouseUp(int id, int x, int y);

  QVariant pluginCall(int id, int function, const QVariant & args);

protected:
  Py_UNICODE* convert(const QString& text);

  GuiScriptingBindings * callBack;

  //PyObject *compileCode(string code);

  PyObject *globals;
  //PyObject *py2perlRegExp;
};
#endif
