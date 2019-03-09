/*
    Copyright (C) 2010-2011  Michiel Jansen

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

#include "RGGoogleMap.h"
#include "RGSettings.h"

#include <QtGui>

/**
 * Workaround with google api, dragging the map doesn't work,
 * emulating Chrome, fixes this.
 */
class myWebPage : public QWebEnginePage
{
protected:

#if 0
		QString userAgentForUrl(const QUrl& ) const
		{
        return "Chrome/1.0";
    }

		void javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID )
		{
			qDebug() << "Javascript: " << sourceID << ":" << lineNumber << ":" << message;
		}
#endif
};

RGGoogleMap::RGGoogleMap(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QFile file("google-maps-template.html");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QFileInfo fileInfo(file.fileName());
		qDebug() << "I can't open google-maps-template.html !!!";
        qDebug() << "Path:" << fileInfo.absoluteFilePath();
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd()) {
		m_html_template.append(in.readLine());
	}

	ui.progressBar->hide();
	ui.progressBar->setRange(0, 100);
    ui.spinBoxX->setValue(RGSettings::getGMXResolution());
    ui.spinBoxY->setValue(RGSettings::getGMYResolution());

    //ui.webView->setPage(new myWebPage());

	//Init map resolution
	on_fixButton_clicked(true);
}
RGGoogleMap::~RGGoogleMap()
{
    qDebug() << "~RGGoogleMap() destroyed";
    if(ui.webView->page() != nullptr){
        qDebug() << "RGGoogleMap destorying webview->page";
        ui.webView->page()->~QWebEnginePage();
    }
    if(ui.webView != nullptr){
        qDebug() << "RGGoogleMap destorying webview";
        ui.webView->close();
    }
}
void RGGoogleMap::accept()
{
	m_map = QPixmap(ui.webView->size());
	ui.webView->render(&m_map);

    RGSettings::setGMXResolution(ui.spinBoxX->value());
    RGSettings::setGMYResolution(ui.spinBoxY->value());
    qDebug() << "Closing dialog by accepted";
    if(ui.webView->page() != nullptr){
        qDebug() << "RGGoogleMap destorying webview->page";
        ui.webView->page()->~QWebEnginePage();
    }
    if(ui.webView != nullptr){
        qDebug() << "RGGoogleMap destorying webview";
        ui.webView->close();
    }


	QDialog::accept();
}
void RGGoogleMap::rejected()
{
    qDebug() << "RGGoogleMap::rejected()";
    if(ui.webView->page() != nullptr){
        qDebug() << "RGGoogleMap destorying webview->page";
        ui.webView->page()->~QWebEnginePage();
    }
    if(ui.webView != nullptr){
        qDebug() << "RGGoogleMap destorying webview";
        ui.webView->close();
    }
    QDialog::reject();
}

void RGGoogleMap::on_goButton_clicked(bool)
{
	//This is what we still accept:
	//http://maps.google.nl/?ie=UTF8&ll=52.36428,4.847116&spn=0.035902,0.077162&z=14
	//this is what we expect since the new google maps version
	//https://www.google.nl/maps/@52.374716,4.898623,12z

	QString manUrl = ui.lineEdit->text();
    QUrl url = manUrl;
	QString latlon;
	QString zoom;
	if (url.hasFragment() || url.host().contains("google"))
	{
        QUrlQuery query(url);
        latlon = query.queryItemValue ("ll");
        zoom = query.queryItemValue ("z");
		if (latlon.isEmpty() || zoom.isEmpty())
		{
			//Now try the new google maps URL format (the construction is not supported by QUrl, so parse the URL manually
			//https://www.google.nl/maps/@52.374716,4.898623,12z
			QStringList part1 = manUrl.split("/@");
			if (part1.size() == 2)
			{
				QStringList locItems = part1[1].split(',');
				if (locItems.size() > 2)
				{
					latlon = locItems[0] + "," + locItems[1];
					zoom = locItems[2].mid(0, locItems[2].indexOf('z'));
				}
			}
		}
	}
	if (latlon.isEmpty() || zoom.isEmpty())
	{
		QMessageBox::warning(this, tr("Web Test"),
												 tr("URL should have format similar like this:\n"
														"http://maps.google.nl/?ie=UTF8&ll=52.36428,4.847116&spn=0.035902,0.077162&z=14\n"
														"OR\n"
														"https://www.google.nl/maps/@52.374716,4.898623,12z\n"
														"Copy it from the paste link option from Google Maps in your browser."));
		return;
	}

    if(ui.webView->page() != nullptr){
        qDebug() << "RGGoogleMap destorying webview->page";
        ui.webView->page()->~QWebEnginePage();
        ui.webView->setPage(new myWebPage());
    }
	ui.webView->setHtml(genHtml(latlon, zoom));
	ui.webView->reload();
}

void RGGoogleMap::on_fixButton_clicked(bool)
{
	ui.webView->setFixedSize(QSize(ui.spinBoxX->value(), ui.spinBoxY->value()));
}

void RGGoogleMap::on_webView_loadFinished ( bool )
{
	ui.progressBar->hide();
}

void RGGoogleMap::on_webView_loadProgress ( int progress )
{
	ui.progressBar->setValue(progress);

}

void RGGoogleMap::on_webView_loadStarted ()
{
	ui.progressBar->show();
}

QString RGGoogleMap::genHtml(const QString &latlon, const QString &zoom) const
{
	qDebug() << "Replace: LATLON = " << latlon << "; zoom = " << zoom;
	QString html = m_html_template;
	html.replace("LATLON", latlon);
	html.replace("ZOOM", zoom);

#if 0
	QFile file("out.html");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream out(&file);
		out << html;
	}
#endif

	return html;
}

