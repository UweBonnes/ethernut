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

#include <stdlib.h>

#include "settings.h"

Settings::Settings()
{
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
	QRegExp nutFolder( "[\\\\/]nut[\\\\/]?$" );

	// First look inside current folder and children.
	QDirIterator it( startPoint, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks );
	while( it.hasNext() )
	{
		QString folder = it.next();
		if ( folder.contains( nutFolder ) )
		{
			QDir nut( folder );
			if ( nut.exists( filename ) )
				return QDir::current().relativeFilePath( QDir( folder ).absoluteFilePath( filename ) );
			else
				return QString();
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
	QRegExp nutFolder( "[\\\\/]nut[\\\\/]?$" );

	// If it is in our parent folder path.
	if ( QDir::current().absolutePath().contains( QRegExp("[\\\\/]nut[\\\\/]") ) )
	{
		QString path = QDir::current().absolutePath();
		path.chop( path.length() - path.indexOf( QRegExp("[\\\\/]nut[\\\\/]") ) );
		return findRelativePathDepthSearch( filename, QDir( path ) );
	}
	
	// Now look for parents and siblings.
	QDir current( QDir::current() );
	current.cdUp(); // Already visited current.

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
	/* Get source path */
	QString srcpath = findRelativePath("os/version.c");
	if ( srcpath.contains( QRegExp("[\\\\/]nut[\\\\/]") ) )
		srcpath.truncate( srcpath.lastIndexOf("nut") + 3 );
	else
		srcpath.truncate( srcpath.lastIndexOf("os/version.c") );

	repositoryFile = srcpath + "/conf/repository.nut";

	QSettings settings;
	m_configFileName = settings.value("settings/configFileName").toString();
	m_buildPath = settings.value("buildPath", "nutbld").toString();
	m_includePath = settings.value("includePath").toStringList();
	m_installPath = settings.value("installPath").toString();
	m_sourceDir = settings.value("sourceDirectory", srcpath).toString();
	m_appDir = settings.value("applicationDirectory", "nutapp").toString();
	m_targetPlatform = settings.value("targetPlatform").toString();
	m_toolPath = settings.value("toolPath").toString();
	m_programmer = settings.value("programmer").toString();
	
	setConfigFileName( fileName );

	return true;
}

void Settings::save()
{
	QSettings settings;
	settings.setValue("settings/configFileName", m_configFileName);
	settings.setValue("buildPath", m_buildPath);
	settings.setValue("includePath", m_includePath);
	settings.setValue("installPath", m_installPath);
	settings.setValue("sourceDirectory", m_sourceDir);
	settings.setValue("applicationDirectory", m_appDir);
	settings.setValue("targetPlatform", m_targetPlatform);
	settings.setValue("toolPath", m_toolPath);
	settings.setValue("programmer", m_programmer);
}
