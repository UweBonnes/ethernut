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

#include <QDir>
#include <QRegExp>
#include <QDirIterator>
#include <QSettings>
#include <QCryptographicHash>
#include <QFileDialog>

#include <stdlib.h>

#include "settings.h"

Settings::Settings()
{
	m_multipleConfigs = false;
	::atexit( Settings::destroy );
}

Settings::~Settings()
{

}

/*!
	Returns the Settings singleton instance.
*/
Settings* Settings::instance()
{
	static Settings* theInstance = new Settings;
	return theInstance;
}

/*!
	\internal
	This method is called by atexit() to destroy the singleton
*/
void Settings::destroy()
{
	delete instance();
}

/*!
	\internal
	Find the relative path for the \a filename relative to our path,
	with a nut folder inside. This performs a deep pattern search, 
	looking for sub folders.
*/
QString Settings::findRelativePathDepthSearch( const QString& filename, const QDir& startPoint )
{
	// First look inside current folder and children.
	QDirIterator it( startPoint, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks );
	while( it.hasNext() )
	{
		QString folder = it.next();
		if ( folder.contains( QRegExp("/nut/?$") ) )
		{
			QDir nut( folder );
			if ( nut.exists( filename ) )
				return QDir::current().relativeFilePath( QDir( folder ).absoluteFilePath( filename ) );
		}
	}
	return QString();
}

/*!
	Find the relative path for the \a filename relative to
	our path, with a nut folder inside.
*/
QString Settings::findRelativePath( const QString& filename )
{
	// If it is in our parent folder path.
	if ( QDir::current().absolutePath().contains("/nut/") )
	{
		QString path = QDir::current().absolutePath();
		path.chop( path.length() - path.indexOf("/nut/") );
		return findRelativePathDepthSearch( filename, QDir( path ) );
	}
	
	// Now look for parents and siblings.
	QDir current( QDir::current() );

	while ( !current.isRoot() )
	{
		QString result = findRelativePathDepthSearch( filename, current );
		if ( result.isEmpty() )
			current.cdUp();
		else
			return result;
	}
	return QString();
}

bool Settings::load( const QString& fileName /*= QString() */ )
{
	QString basePath;
	QString baseName;
	QSettings settings;
	if ( multipleConfigs() && !fileName.isEmpty() )
	{
		QByteArray hash = QCryptographicHash::hash( fileName.toLocal8Bit(), QCryptographicHash::Md5 );
		settings.beginGroup( hash.toHex() );
	}
	if (!fileName.isEmpty()) {
		baseName = QFileInfo(fileName).baseName();
		basePath = QFileInfo(fileName).canonicalPath();
		if (basePath.contains("nut/conf", Qt::CaseSensitive)) {
			basePath.truncate(basePath.lastIndexOf("/nut/conf"));
			m_sourceDir = basePath + "/nut";
			settings.setValue("sourceDirectory", m_sourceDir);
		}
	}
	if(!settings.contains("sourceDirectory")) {
		/* Ask the user for the source path */
		QString  srcpath;
		srcpath = QFileDialog::getExistingDirectory
			( 0, tr("Ethernut source directory"),
			getenv("HOME"),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if ( srcpath.isEmpty() )  {// User canceled
			/* Get source path */
			//message( tr("Scanning disk for ethernut source") );
			if ( srcpath.contains("/nut/") )
				srcpath.truncate( srcpath.lastIndexOf("nut") + 3 );
			else
				srcpath.truncate( srcpath.lastIndexOf("/os/version.c") );
		}
		else
			m_sourceDir = srcpath;
		/* Set the value or otherwise the setting box doesn't pick up the new srcpath*/
		settings.setValue("sourceDirectory", m_sourceDir);
	}
	else
		m_sourceDir = settings.value("sourceDirectory", "").toString();


	repositoryFile = m_sourceDir + "/conf/repository.nut";

	m_multipleConfigs = settings.value("settings/multipleconfig").toBool();
	m_configFileName = settings.value("settings/configFileName").toString();

	basePath = basePath + "/" + baseName;
	m_buildPath = basePath + "/nutbld";
	settings.value("buildPath", m_buildPath);
	m_installPath = settings.value("installPath").toString();
	m_appDir = basePath + "/nutapp";
	settings.value("applicationDirectory", m_appDir);

	QString defaultToolPath;
#if defined( Q_OS_WIN32 )
	defaultToolPath = QDir(m_sourceDir + "/tools/win32").absolutePath();
#endif
	m_toolPath = settings.value("toolPath", defaultToolPath).toString();
	m_absolutePathInSamples = settings.value("absolutePathInSamples", true).toBool();
	m_clearLogBeforeBuild = settings.value("clearLogBeforeBuild", true).toBool();
	m_verboseBuild = settings.value("verboseBuild", false).toBool();
	setConfigFileName( fileName );

	return true;
}

void Settings::save()
{
	QSettings settings;
	settings.setValue("settings/configFileName", m_configFileName);
	settings.setValue("settings/multipleconfig", m_multipleConfigs);

	if ( multipleConfigs() && !configFileName().isEmpty() )
	{
		QByteArray hash = QCryptographicHash::hash( configFileName().toLocal8Bit(), QCryptographicHash::Md5 );
		settings.beginGroup( hash.toHex() );
	}

	settings.setValue("buildPath", m_buildPath);
	settings.setValue("installPath", m_installPath);
	settings.setValue("sourceDirectory", m_sourceDir);
	settings.setValue("applicationDirectory", m_appDir);
	settings.setValue("toolPath", m_toolPath);
	settings.setValue("absolutePathInSamples", m_absolutePathInSamples);
	settings.setValue("clearLogBeforeBuild", m_clearLogBeforeBuild);
	settings.setValue("verboseBuild", m_verboseBuild);
}
