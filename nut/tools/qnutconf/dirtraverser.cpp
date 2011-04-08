
#include "dirtraverser.h"

#include <QDir>
#include <QFile>
#include <QDirIterator>

DirTraverser::DirTraverser()
{
}

DirTraverser::~DirTraverser()
{
	foreach( AbstractFileCopyFilter* filter, filters )
	{
		delete filter;
	}
	filters.clear();
}

void DirTraverser::copyDir( const QString& src, const QString& dest )
{
	//Check whether the dir directory exists
	if( QDir( src ).exists() ) 
	{
		if( !QDir( dest ).exists() ) 
			QDir().mkpath( dest );

		//Construct an iterator to get the entries in the directory
		QDirIterator dirIterator( src, QDir::AllEntries | QDir::NoDotAndDotDot );
		while ( dirIterator.hasNext() ) 
		{
			QString item = dirIterator.next();
			QString fileName = dirIterator.fileName();
			QFileInfo fileInfo = dirIterator.fileInfo();

			//If entry is a file copy it
			if ( fileInfo.isFile() )
			{
				bool done = false;

				foreach( AbstractFileCopyFilter* filter, filters ) {
					done = filter->onFile( fileInfo, dest );
					if ( done )
						break;
				}
				if ( !done )
					QFile::copy( item, dest + "/" + fileName );
			}
			else
				copyDir( item, dest + "/" + fileName );
		}
	}
	else
	{
		qWarning( qPrintable(QString("DirTraverser::copyDir - src folder [%1] doesn't exist").arg(src)) );
	}
}

void DirTraverser::insertFilter( AbstractFileCopyFilter* filter )
{
	filters.insert( 0, filter );
}

bool AppDirCopyFilter::onFile( const QFileInfo &fileInfo, const QString& dest)
{
	Q_UNUSED(dest);
	QString fileName = fileInfo.fileName();
	const char* exclude_pattern[] = {
		"Makeburn\\..*",
		"Makedefs\\..*",
		"Makerules\\..*",
		"Makevars\\..*"
	};

	for (int i = 0; i < sizeof(exclude_pattern) / sizeof(exclude_pattern[0]); i++)
		if ( QRegExp(exclude_pattern[i]).exactMatch(fileName) )
			return true;

	return false;
}
