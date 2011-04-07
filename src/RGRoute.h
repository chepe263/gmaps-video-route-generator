/*
    Copyright (C) 2009-2011  Michiel Jansen
    Copyright (C) 2010-2011  Fabien Valthier

  This file is part of Route Generator.

    Route Generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RGROUTE_H
#define RGROUTE_H

#include "RGGraphicsObjectUndo.h"
#include "RGVehicleList.h"
#include "RGRouteUi.h"
#include "RGPath.h"
#include "RGEditPath.h"

class RGRoute : public RGGraphicsObjectUndo
{
  Q_OBJECT
public:
  explicit RGRoute(QGraphicsItem *parent = 0);
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
  QWidget *widgetSettings();
  void setSmoothCoef(int);
  void setEditMode(bool);
  int countFrames();
  void setCurrentFrame(int);
  void setIconlessBeginEndFrames(bool);
  void setNewPoints(QList<QPoint>);
  virtual void undoredo(QVariant=0);

signals:
  void canGenerate(bool);
  void sceneRectChanged();
  void newUndoable(RGGraphicsObjectUndo *,QVariant);

public slots:
  void on_playbackChanged(bool);
  void clearPath();

private slots:
  void on_penChanged(const QPen & pen);
  void on_totalTimeChecked(bool);
  void on_smoothPathChecked(bool);
  void on_routeTimeChanged(int);
  void on_vehicleChanged();
  void on_pathChanged(QList<QPoint>,bool);

private:
  void updateVehicle();

protected:
  RGRouteUi *mRouteUi;
  RGVehicleList *mVehicleList;
  RGPath  *mPath;
  RGEditPath * mEditPath;
  bool mIconlessBeginEndFrames;
  bool mPlayback;
  bool mEditMode;
};

#endif // RGROUTE_H
