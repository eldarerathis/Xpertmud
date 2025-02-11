#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include <qfiledialog.h>
#include "kurl.h"
#include <qapplication.h>

class KFileDialog: public QFileDialog {
  Q_OBJECT
 public:
  static KURL getOpenURL(KURL initially = QString(),
		     const QString & filter = QString(), 
		     QWidget * parent = 0, const QString name = QString()) {
    if (initially.isEmpty()) {
       initially=qApp->applicationDirPath();
    }
    return getOpenFileName(parent, name, initially.url(), filter);
  }
  static KURL getSaveURL(KURL  initially = QString(),
		     const QString & filter = QString(), 
		     QWidget * parent = 0, const QString name = QString()) {
    if (initially.isEmpty()) {
       initially=qApp->applicationDirPath();
    }
    return getSaveFileName(parent, name, initially.url(), filter);
  }
};

#endif
