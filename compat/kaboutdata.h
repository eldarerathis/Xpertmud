#ifndef KABOUTDATA_H
#define KABOUTDATA_H

#include <qstring.h>
#include <utility>
#include <vector>
#ifdef WIN32
#pragma warning( disable : 4100 )
#endif

#define VERSION 1

class KAboutData {
 public:
  enum ELicense { License_GPL };

  KAboutData() {}

  KAboutData(const QString& name,
	     const QString& name2,
	     int version,
	     const QString& desc,
	     ELicense lic,
	     const QString& cop,
	     int, int,
	     const QString &addr) {

		cr = cop;
  }

  void addAuthor(const QString& name,
		   int, const QString & addr) {

	auths.push_back(std::pair<QString, QString>{name, addr});
  }
  void addCredit(const QString& name,
		 const QString& work,
		 const QString& addr,
		   const QString& web) {}

  QString programName() const { return "xpertmud"; }
  QString version() const { return "Win32"; }
  QString copyright() const { return cr; }
  const std::vector<std::pair<QString, QString>>* authors() { return &auths; } 

  private:
	QString cr;
	std::vector<std::pair<QString, QString>> auths;
};

#endif
