/***************************************************************************
 SocNetV: Social Network Visualizer
 version: 2.2
 Written in Qt

                         dialogranderdosrenyi.h  -  description
                             -------------------
    copyright         : (C) 2005-2017 by Dimitris B. Kalamaras
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

#ifndef DIALOGRANDERDOSRENYI_H
#define DIALOGRANDERDOSRENYI_H

#include <QDialog>

#include "ui_dialogranderdosrenyi.h"

class DialogRandErdosRenyi  : public QDialog
{
    Q_OBJECT
public:
    explicit DialogRandErdosRenyi(QWidget *parent=0, const float eprob = 0);

public slots:
    void checkErrors();
    void gatherData();
    void gnmModel();
    void gnpModel();
    void setModeDirected();
    void setModeUndirected();
    void setDiag();

signals:
    void userChoices( const int nodes,
                      const QString model,
                      const int edges,
                      const float eprob,
                      const QString mode,
                      const bool diag);
private:
    QString model;
    QString mode;
    int nodes, edges;

    bool diag;
    Ui::DialogRandErdosRenyi ui;
};

#endif
