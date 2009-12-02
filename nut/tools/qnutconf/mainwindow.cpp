/*
 * Copyright (C) 2009 by Comm5 Tecnologia Ltda. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COMM5 TECNOLOGIA LTDA AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

#include <QTime>
#include <QTimer>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "settingsdialog.h"
#include "nutcomponentmodel.h"
#include "nutcomponentdelegate.h"
#include "settings.h"
#include "builder.h"

MainWindow::MainWindow()
{
	ui.setupUi( this );
	Settings::instance()->load();
	model = new NutComponentModel(Settings::instance()->repository(), this);

	ui.componentTree->setModel( model );
	ui.componentTree->setItemDelegate( new NutComponentDelegate( model ) );

	connect( model, SIGNAL(errorMessage(const QString&)), SLOT(message(const QString&)) );
	connect( model, SIGNAL(message(const QString&)), SLOT(message(const QString&)) );
	connect( ui.componentTree->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), SLOT(updateView(const QModelIndex&, const QModelIndex&)) );

	readSettings();

	message( tr("Nut/OS Configurator Version %1").arg(NUTCONF_VERSION_STR) );
	message( tr("Linked to Qt %1").arg(QT_VERSION_STR) );
	message( tr("Working in %1").arg( QDir::current().absolutePath() ) );

	connect( Builder::instance(), SIGNAL(message(const QString&)), SLOT(message(const QString&)) );
	connect( Builder::instance(), SIGNAL(done(int)), SLOT(buildFinished(int)) );

	// Run File->Open as soon as we enter the event loop
	QTimer::singleShot( 100, this, SLOT(on_actionOpen_triggered()) );
}

MainWindow::~MainWindow()
{
	writeSettings();
}

void MainWindow::readSettings()
{
	QSettings settings;
	QPoint pos = settings.value("pos", QPoint(50, 50)).toPoint();
	QSize size = settings.value("size", QSize(550, 350)).toSize();
	resize(size);
	move(pos);
}

void MainWindow::writeSettings()
{
	QSettings settings;
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Nut/OS Configuration (*.conf)") );
	if ( !fileName.isEmpty() )
	{
		QApplication::setOverrideCursor( Qt::BusyCursor );
		if ( !model->openConfig( fileName ) )
		{
			QApplication::restoreOverrideCursor();
			QMessageBox::critical(this, tr("Erro"), tr("There was a problem opening the file, please check the log message"));
		}
		else
			Settings::instance()->load( fileName );
		QApplication::restoreOverrideCursor();
	}
}

void MainWindow::on_actionSave_triggered()
{
	model->saveConfig( Settings::instance()->configFileName() );
}

void MainWindow::on_actionSave_as_triggered()
{
	QString fileName = QFileDialog::getOpenFileName( this, tr("Browse"), Settings::instance()->repository(), tr("Nut/OS Configuration (*.conf)") );
	if ( fileName.isEmpty() )
		return;

	Settings::instance()->setConfigFileName( fileName );
	model->saveConfig( fileName );
}

void MainWindow::on_actionExit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_actionSettings_triggered()
{
	SettingsDialog dialog;
	dialog.exec();
}

void MainWindow::on_actionBuild_Nut_OS_triggered()
{
	/*
	 * Create a message box containing all relevant information and
	 * let the user decide if he really wants that.
	 */
	QString question = tr(
							"\nBuild directory:   %1"
							"\nTarget platform:   %2"
							"\nInstall directory: %3"
							"\n\nDo you want to build the Nut/OS libraries?\n"
							).arg( Settings::instance()->buildPath(), Settings::instance()->targetPlatform(), Settings::instance()->installPath() );
	if ( QMessageBox::question( this, tr("Build Nut/OS"), question, QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
		return; 
	
	// Measure build time
	time.start();

	// Generate the build tree
	model->generateBuildTree();

	/* Clean up any previous build */
	Builder::instance()->build( "clean" );
	/* Make all */
	Builder::instance()->build( "all" );

	ui.actionBuild_Nut_OS->setEnabled( false );
}

void MainWindow::buildFinished( int exitCode )
{
	if ( !exitCode )
		message( tr("Build finished successfully in %1s" ).arg( time.elapsed() / 1000 ) );
	else
		message( tr("Build failed") );

	ui.actionBuild_Nut_OS->setEnabled( true );
}

void MainWindow::updateView( const QModelIndex& current, const QModelIndex& previous )
{
	Q_UNUSED(previous);

	QVariant data = model->data( current, NutComponentModel::Description );
	ui.descriptionPanel->setText( data.toString() );
}

void MainWindow::message( const QString& msg )
{
	ui.logPanel->append( QTime::currentTime().toString("HH:mm:ss: ") +  msg );
}

