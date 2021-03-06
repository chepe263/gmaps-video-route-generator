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


#ifndef RGENCFFMPEG_H
#define RGENCFFMPEG_H

#include "RGEncVideo.h"

class RGEncFFmpeg : public RGEncVideo
{
public:
  RGEncFFmpeg(QWidget *parent = 0);

  virtual void updateFromSettings();
  virtual void saveInSettings();
  virtual void generateMovie(const QString &dirName, const QString &filePrefix);
  virtual QString encoderName();
  virtual QString encoderExecBaseName();

protected:
	virtual bool initCodecs();

private:
  int mBitRate;
};

#endif // RGENCFFMPEG_H
