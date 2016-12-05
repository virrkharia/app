/***************************************************************************
 SocNetV: Social Network Visualizer
 version: 2.2
 Written in Qt
 
                         pearsoncorrelationdialog.h  -  description
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

#ifndef PEARSONCORRELATIONDIALOG_H
#define PEARSONCORRELATIONDIALOG_H

#include <QDialog>
#include "ui_pearsoncorrelationdialog.h"
 

class PearsonCorrelationDialog: public QDialog
{
	Q_OBJECT
public:
    PearsonCorrelationDialog (QWidget *parent = 0);
    ~PearsonCorrelationDialog();
public slots:
	void gatherData();
signals:
	void userChoices(QString);	
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PearsonCorrelationDialog ui;
    QStringList matrixList, variablesLocationList;

};



#endif