/* This file is part of the KDE libraries
   Copyright (C) 2000 Max Judin <novaprint@mtu-net.ru>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
#include "kdockwidget.h"
#include "kdockwidget_private.h"

#include <QPainter>
#include <QResizeEvent>

KDockSplitter::KDockSplitter(QWidget *parent, const QString name, Qt::Orientation orient, int pos, bool highResolution)
: QWidget(parent)
{
  setObjectName(name);
  divider = 0L;
  child0 = 0L;
  child1 = 0L;
  orientation = orient;
  mOpaqueResize = false;
  mKeepSize = false;
  mHighResolution = highResolution;
  setSeparatorPos( pos, false );
  initialised = false;
}

void KDockSplitter::activate(QWidget *c0, QWidget *c1)
{
  if ( c0 ) child0 = c0;
  if ( c1 ) child1 = c1;

  setupMinMaxSize();

  if (divider) delete divider;
  divider = new QFrame(this);
  divider->setObjectName("pannerdivider");
  divider->setFrameStyle(QFrame::Panel | QFrame::Raised);
  divider->setLineWidth(1);
  divider->raise();

  if (orientation == Qt::Horizontal)
    divider->setCursor(QCursor(Qt::SizeVerCursor));
  else
    divider->setCursor(QCursor(Qt::SizeHorCursor));

  divider->installEventFilter(this);

  initialised= true;

  updateName();

  divider->show();
  resizeEvent(0);
}

void KDockSplitter::setupMinMaxSize()
{
  // Set the minimum and maximum sizes
  int minx, maxx, miny, maxy;
  if (orientation == Qt::Horizontal) {
    miny = child0->minimumSize().height() + child1->minimumSize().height()+4;
    maxy = child0->maximumSize().height() + child1->maximumSize().height()+4;
    minx = (child0->minimumSize().width() > child1->minimumSize().width()) ? child0->minimumSize().width() : child1->minimumSize().width();
    maxx = (child0->maximumSize().width() > child1->maximumSize().width()) ? child0->maximumSize().width() : child1->maximumSize().width();

    miny = (miny > 4) ? miny : 4;
    maxy = (maxy < 2000) ? maxy : 2000;
    minx = (minx > 2) ? minx : 2;
    maxx = (maxx < 2000) ? maxx : 2000;
  } else {
    minx = child0->minimumSize().width() + child1->minimumSize().width()+4;
    maxx = child0->maximumSize().width() + child1->maximumSize().width()+4;
    miny = (child0->minimumSize().height() > child1->minimumSize().height()) ? child0->minimumSize().height() : child1->minimumSize().height();
    maxy = (child0->maximumSize().height() > child1->maximumSize().height()) ? child0->maximumSize().height() : child1->maximumSize().height();

    minx = (minx > 4) ? minx : 4;
    maxx = (maxx < 2000) ? maxx : 2000;
    miny = (miny > 2) ? miny : 2;
    maxy = (maxy < 2000) ? maxy : 2000;
  }
  setMinimumSize(minx, miny);
  setMaximumSize(maxx, maxy);
}

void KDockSplitter::deactivate()
{
  if (divider) delete divider;
  divider = 0L;
  initialised= false;
}

void KDockSplitter::setSeparatorPos(int pos, bool do_resize)
{
  xpos = pos;
  if (do_resize)
    resizeEvent(0);
}

int KDockSplitter::separatorPos()
{
  return xpos;
}

void KDockSplitter::resizeEvent(QResizeEvent *ev)
{
  if (initialised){
    int factor = (mHighResolution)? 10000:100;
    // real resize event, recalculate xpos
    if (ev && mKeepSize && isVisible()) {
      if (orientation == Qt::Horizontal) {
        if (ev->oldSize().height() != ev->size().height())
          xpos = factor * checkValue( child0->height()+1 ) / height();
      } else {
        if (ev->oldSize().width() != ev->size().width())
          xpos = factor * checkValue( child0->width()+1 ) / width();
      }
    }
    int position = checkValue( (orientation == Qt::Vertical ? width() : height()) * xpos/factor );
    if (orientation == Qt::Horizontal){
      child0->setGeometry(0, 0, width(), position);
      child1->setGeometry(0, position+4, width(), height()-position-4);
      divider->setGeometry(0, position, width(), 4);
    } else {
      child0->setGeometry(0, 0, position, height());
      child1->setGeometry(position+4, 0, width()-position-4, height());
      divider->setGeometry(position, 0, 4, height());
    }
  }
}

int KDockSplitter::checkValue( int position )
{
  if (initialised){
    if (orientation == Qt::Vertical){
      if (position < (child0->minimumSize().width()))
        position = child0->minimumSize().width();
      if ((width()-4-position) < (child1->minimumSize().width()))
        position = width() - (child1->minimumSize().width()) -4;
    } else {
      if (position < (child0->minimumSize().height()))
        position = (child0->minimumSize().height());
      if ((height()-4-position) < (child1->minimumSize().height()))
        position = height() - (child1->minimumSize().height()) -4;
    }
  }

  if (position < 0) position = 0;

  if ((orientation == Qt::Vertical) && (position > width()))
    position = width();
  if ((orientation == Qt::Horizontal) && (position > height()))
    position = height();

  return position;
}

bool KDockSplitter::eventFilter(QObject *o, QEvent *e)
{
  QMouseEvent *mev;
  bool handled = false;
  int factor = (mHighResolution)? 10000:100;

  switch (e->type()) {
    case QEvent::MouseMove:
      mev= (QMouseEvent*)e;
      child0->setUpdatesEnabled(mOpaqueResize);
      child1->setUpdatesEnabled(mOpaqueResize);
      if (orientation == Qt::Horizontal) {
        if (!mOpaqueResize) {
          int position = checkValue( mapFromGlobal(mev->globalPos()).y() );
          divider->move( 0, position );
        } else {
          xpos = factor * checkValue( mapFromGlobal(mev->globalPos()).y() ) / height();
          resizeEvent(0);
          divider->repaint();
        }
      } else {
        if (!mOpaqueResize) {
          int position = checkValue( mapFromGlobal(QCursor::pos()).x() );
          divider->move( position, 0 );
        } else {
          xpos = factor * checkValue( mapFromGlobal( mev->globalPos()).x() ) / width();
          resizeEvent(0);
          divider->repaint();
        }
      }
      handled= true;
      break;
    case QEvent::MouseButtonRelease:
      child0->setUpdatesEnabled(true);
      child1->setUpdatesEnabled(true);
      mev= (QMouseEvent*)e;
      if (orientation == Qt::Horizontal){
        xpos = factor* checkValue( mapFromGlobal(mev->globalPos()).y() ) / height();
        resizeEvent(0);
        divider->repaint();
      } else {
        xpos = factor* checkValue( mapFromGlobal(mev->globalPos()).x() ) / width();
        resizeEvent(0);
        divider->repaint();
      }
      handled= true;
      break;
    default:
      break;
  }
  return (handled) ? true : QWidget::eventFilter( o, e );
}

bool KDockSplitter::event( QEvent* e )
{
  if ( e->type() == QEvent::LayoutRequest ){
    // change children min/max size
    setupMinMaxSize();
    setSeparatorPos(xpos);
  }
  return QWidget::event(e);
}

QWidget* KDockSplitter::getAnother( QWidget* w )
{
  return ( w == child0 ) ? child1 : child0;
}

void KDockSplitter::updateName()
{
  if ( !initialised ) return;

  QString new_name = QString( child0->objectName() ) + "," + child1->objectName();
  parentWidget()->setObjectName( new_name );
  parentWidget()->setWindowTitle( child0->windowTitle() + "," + child1->windowTitle() );
  parentWidget()->repaint();

  ((KDockWidget*)parentWidget())->firstName = child0->objectName();
  ((KDockWidget*)parentWidget())->lastName = child1->objectName();
  ((KDockWidget*)parentWidget())->splitterOrientation = orientation;

  QWidget* p = parentWidget()->parentWidget();
  if ( p != 0L && p->inherits("KDockSplitter" ) )
    ((KDockSplitter*)p)->updateName();
}

void KDockSplitter::setOpaqueResize(bool b)
{
  mOpaqueResize = b;
}

bool KDockSplitter::opaqueResize() const
{
  return mOpaqueResize;
}

void KDockSplitter::setKeepSize(bool b)
{
  mKeepSize = b;
}

bool KDockSplitter::keepSize() const
{
  return mKeepSize;
}

void KDockSplitter::setHighResolution(bool b)
{
  if (mHighResolution) {
    if (!b) xpos = xpos/100;
  } else {
    if (b) xpos = xpos*100;
  }
  mHighResolution = b;
}

bool KDockSplitter::highResolution() const
{
  return mHighResolution;
}


/*************************************************************************/
KDockButton_Private::KDockButton_Private( QWidget *parent, const QString name )
:QPushButton( parent )
{
  setObjectName(name);
  moveMouse = false;
  setFocusPolicy( Qt::NoFocus );
}

KDockButton_Private::~KDockButton_Private()
{
}

void KDockButton_Private::drawButton( QPainter* p )
{
  p->fillRect( 0,0, width(), height(), QBrush(palette().window() ));
  p->drawPixmap( (width() - iconSize().width()) / 2, (height() - iconSize().height()) / 2, icon().pixmap(iconSize()) );
  if ( moveMouse && !isDown() ){
    p->setPen( Qt::white );
    p->drawLine(0, height() - 1, 0, 0);
    p->drawLine( 0, 0, width() - 1, 0);

    p->setPen( palette().dark().color() );
    p->drawLine( width() - 1, 0, width() - 1, height() - 1);
    p->drawLine( width() - 1, height() - 1, 0, height() - 1 );
  }
  if ( isChecked() || isDown() ){
    p->setPen( palette().dark().color() );
    p->drawLine(0, height() - 1, 0, 0);
    p->drawLine( 0, 0, width() - 1, 0);

    p->setPen( Qt::white );
    p->drawLine( width() - 1, 0, width() - 1, height() - 1);
    p->drawLine( width() - 1, height() - 1, 0, height() - 1 );

  }
}

void KDockButton_Private::enterEvent( QEvent * )
{
  moveMouse = true;
  repaint();
}

void KDockButton_Private::leaveEvent( QEvent * )
{
  moveMouse = false;
  repaint();
}

/*************************************************************************/
KDockWidgetPrivate::KDockWidgetPrivate()
  : QObject()
  ,index(-1)
  ,splitPosInPercent(50)
  ,pendingFocusInEvent(false)
  ,blockHasUndockedSignal(false)
{
#ifndef NO_KDE2
  windowType = NET::Normal;
#endif

  _parent = 0L;
  transient = false;
}

KDockWidgetPrivate::~KDockWidgetPrivate()
{
}
