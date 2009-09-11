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

#include "settingsdialog.h"
#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	// Repository tab
	ui.e_RepositoryFile->setText( Settings::instance()->repository() );

	// Build tab
	ui.e_SourceDirectory->setText( Settings::instance()->sourceDir() );
	ui.e_BuildDirectory->setText( Settings::instance()->buildPath() );
	ui.e_InstallDirectory->setText( Settings::instance()->installPath() );

	populatePlatform();
}

SettingsDialog::~SettingsDialog()
{

}

/*!
	Fills the platform selection combo box.
	Scans the source directory for files with a base name of 'Makedefs'. 
	The extensions of all files found are added to the combo box.
*/
void SettingsDialog::populatePlatform()
{
	QDir src_dir( ui.e_SourceDirectory->text() );
	QString platform = ui.cb_Platform->currentText();

	if ( src_dir.exists() )
	{
		ui.cb_Platform->clear();
		foreach( QString file, src_dir.entryList( QStringList() << "Makedefs.*") )
		{
			ui.cb_Platform->addItem( file.mid( 9 ) );
		}
	}
}

void SettingsDialog::accept()
{
	QDialog::accept();
}