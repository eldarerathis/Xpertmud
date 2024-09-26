/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Tue Jun 19 11:36:10 MEST 2001
    copyright            : (C) 2001 by Ernst Bachmann
    email                : e.bachmann@xebec.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kcmdlineargs.h>
#include <klocale.h>

#include "xpertmud.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
	
static KCmdLineOptions options[] =
{
  { "+[xmud]", I18N_NOOP("The xpertmud bookmark file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{
  KCmdLineArgs::init( argc, argv, NULL );
  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication a;
  Xpertmud *xpertmud = new Xpertmud();
  //a.setMainWidget(xpertmud);
  xpertmud->show();  
  xpertmud->initialize();
  if (a.isRestored() && KMainWindow::canBeRestored(1)) {
    xpertmud->restore(1);
  } else {

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if ( args->count() ) {
      // we only process the first arg
      xpertmud->slotLoadBookmark(args->arg( 0 ));
    
    }
  }
  return a.exec();
}
