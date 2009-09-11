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

#include "nutcomponentmodel.h"

#include <QtGui>

#include "nutcomponentmodel.h"
#include "nutcomponentmodel_p.h"

#include "nutcomponent.h"


class NutComponentModel::NutComponentModelPrivate
{
public:
	NutComponentModelPrivate() : rootItem( 0 ), repository(0), rootComponent(0)
	{

	}

	~NutComponentModelPrivate()
	{
		delete rootItem;
		if ( repository )
			CloseRepository( repository );
		if ( rootComponent )
			ReleaseComponents( rootComponent );
	}

	void addChildItems( NUTCOMPONENT* parentComponent, TreeItem* parentItem, NutComponentModel* model );

	TreeItem *rootItem;
	NUTREPOSITORY* repository;
	NUTCOMPONENT* rootComponent;
	QString repositoryPath;
};

void NutComponentModel::NutComponentModelPrivate::addChildItems( NUTCOMPONENT* parentComponent, TreeItem* parentItem, NutComponentModel* model )
{
	if(parentComponent) {
		parentComponent = parentComponent->nc_child;
		while (parentComponent) {
			TreeItem* item = new TreeItem(parentComponent, model, parentItem);
			parentItem->appendChild( item );

			NUTCOMPONENTOPTION *opts = parentComponent->nc_opts;
			while (opts) {
				TreeItem* childItem = new TreeItem(opts, model, item);
				item->appendChild( childItem );
				opts = opts->nco_nxt;
			}

			addChildItems(parentComponent, item, model);
			parentComponent = parentComponent->nc_nxt;
		}
	}

}

NutComponentModel::NutComponentModel(const QString &path, QObject *parent)
: QAbstractItemModel(parent), d( new NutComponentModelPrivate )
{
	d->rootItem = new TreeItem( 0, 0 );

	QString repositoryPath = path;
	d->repositoryPath = repositoryPath.replace("\\", "/");
}

NutComponentModel::~NutComponentModel()
{
	delete d;
}

bool NutComponentModel::openConfig( const QString& fileName )
{
	if ( !d->repository )
	{
		d->repository = OpenRepository( qPrintable(d->repositoryPath) );
		if ( !d->repository )
		{
			emit errorMessage( QLatin1String(GetScriptErrorString()) );
			return false; // Failed to open repository
		}
		
		d->rootComponent = LoadComponents(d->repository);
		if ( !d->rootComponent )
		{
			emit errorMessage( QLatin1String(GetScriptErrorString()) );
			return false; // Failed to open repository
		}
		
		if(ConfigureComponents(d->repository, d->rootComponent, qPrintable(fileName)))
		{
			emit errorMessage( QLatin1String(GetScriptErrorString()) );
			return false; // Failed to open repository
		}

		if (RefreshComponents(d->repository, d->rootComponent)) 
			emit errorMessage( tr("Conflicting configuration") );
		 else
			emit message( tr("OK") );
	}
	rebuildTree();
	reset();
	return true;
}

int NutComponentModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return d->rootItem->columnCount();
}

QVariant NutComponentModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column(), role);
}

bool NutComponentModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	if ( !index.isValid() )
		return false;

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	if ( role == Qt::EditRole )
	{
		if ( item->setValue( value ) )
		{
			emit dataChanged(index, index);
			return true;
		}
	}
	else if ( role == Qt::CheckStateRole )
	{
		item->setActive( value.toBool() );
		RefreshComponents(d->repository, d->rootComponent);
		emit dataChanged( this->index(0, 0), this->index(rowCount(), 2) );
		return true;
	}
	return false;
}

Qt::ItemFlags NutComponentModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->flags(index.column());
}

QVariant NutComponentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	Q_UNUSED(role);
	return QVariant();
}

QModelIndex NutComponentModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = d->rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex NutComponentModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == d->rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int NutComponentModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = d->rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void NutComponentModel::rebuildTree()
{
	d->rootItem->clear();
	d->addChildItems( d->rootComponent, d->rootItem, this );
}

NUTREPOSITORY* NutComponentModel::repository()
{
	return d->repository;
}

bool NutComponentModel::isOptionActive( const char* name )
{
	NUTCOMPONENTOPTION *opt = findOptionByName(d->rootComponent->nc_child, name);

	if (opt && opt->nco_active) {
		return true;
	}
	return false;

}

NUTCOMPONENTOPTION* NutComponentModel::findOptionByName( NUTCOMPONENT* compo, const char* name )
{
	if (!compo && d->rootComponent)
		compo = d->rootComponent->nc_child;

	while (compo) 
	{
		NUTCOMPONENTOPTION* opts = compo->nc_opts;
		while (opts) {
			if (strcmp(opts->nco_name, name) == 0) {
				return opts;
			}
			opts = opts->nco_nxt;
		}

		if (compo->nc_child && (opts = findOptionByName(compo->nc_child, name)) != 0) {
			return opts;
		}
		compo = compo->nc_nxt;
	}
	return 0;
}

/*!
	Deactivate a list of options.
*/
void NutComponentModel::deactivateOptionList( char **exlist )
{
	if (!exlist)
		return;

	for (int i = 0; exlist[i]; ++i) 
	{
		NUTCOMPONENTOPTION *opt = findOptionByName(d->rootComponent->nc_child, exlist[i]);
		if (opt) 
			opt->nco_active = 0;
	}
}
