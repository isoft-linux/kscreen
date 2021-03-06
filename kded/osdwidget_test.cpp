/*************************************************************************************
*  Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>                        *
*  Copyright (C) 2015 Dan Vrátil <dvratil@redhat.com>                               *
*                                                                                   *
*  This program is free software; you can redistribute it and/or                    *
*  modify it under the terms of the GNU General Public License                      *
*  as published by the Free Software Foundation; either version 2                   *
*  of the License, or (at your option) any later version.                           *
*                                                                                   *
*  This program is distributed in the hope that it will be useful,                  *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
*  GNU General Public License for more details.                                     *
*                                                                                   *
*  You should have received a copy of the GNU General Public License                *
*  along with this program; if not, write to the Free Software                      *
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
*************************************************************************************/

#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>

#include "osdwidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow window;
    OsdWidget widget;
    QObject::connect(&widget, &OsdWidget::displaySwitch,
                    [](Generator::DisplaySwitchAction action) {
                        qDebug() << "OsdWidget::displaySwitch emitted:" << action;
                    });
    window.setCentralWidget(&widget);
    window.show();
    widget.show();
    return app.exec();
}
