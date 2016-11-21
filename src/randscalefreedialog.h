/***************************************************************************
 SocNetV: Social Network Visualizer
 version: 2.2
 Written in Qt

                         randscalefreeddialog.h  -  description
                             -------------------
    copyright         : (C) 2005-2016 by Dimitris B. Kalamaras
    project site      : http://socnetv.org

 ***************************************************************************/

/*******************************************************************************
*     This program is free software: you can redistribute it and/or modify     *
*     it under the terms of the GNU General Public License as published by     *
*     the Free Software Foundation, either version 3 of the License, or        *
*     (at your option) any later version.                                      *
*                                                                              *
*     This program is distributed in the hope that it will be useful,          *
*     but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*     GNU General Public License for more details.                             *
*                                                                              *
*     You should have received a copy of the GNU General Public License        *
*     along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
********************************************************************************/

#ifndef RANDSCALEFREEDIALOG_H
#define RANDSCALEFREEDIALOG_H

#include <QDialog>

#include "ui_randscalefreedialog.h"

class RandScaleFreeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RandScaleFreeDialog(QWidget *parent = 0);

public slots:
    void checkErrors();
    void gatherData();
    void setModeDirected();
    void setModeUndirected();
    void setDiag();

signals:
    void userChoices( const int &nodes,
                      const int &power,
                      const int &initialNodes,
                      const int &edgesPerStep,
                      const float &zeroAppeal,
                      const QString &mode);
private:
    QString mode;
    int nodes; // n
    int initialNodes; // m0
    int edgesPerStep; //m
    int power;
    float zeroAppeal; // a
    bool diag;
    Ui::RandScaleFreeDialog ui;

};

#endif // RANDSCALEFREEDIALOG_H
