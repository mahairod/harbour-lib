/*
 * Copyright (C) 2021-2024 Slava Monich <slava@monich.com>
 * Copyright (C) 2021 Jolla Ltd.
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the
 *     distribution.
 *
 *  3. Neither the names of the copyright holders nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * any official policies, either expressed or implied.
 */

#ifndef HARBOUR_COLOR_EDITOR_MODEL_H
#define HARBOUR_COLOR_EDITOR_MODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>
#include <QtGui/QColor>

class HarbourColorEditorModel :
    public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList colors READ getColors WRITE setColors NOTIFY colorsChanged)
    Q_PROPERTY(int dragPos READ getDragPos WRITE setDragPos NOTIFY dragPosChanged)
    Q_ENUMS(ItemType)

public:
    enum ItemType {
        ColorItem,
        TrashedItem,
        AddItem
    };

    HarbourColorEditorModel(QObject* aParent = Q_NULLPTR);
    ~HarbourColorEditorModel();

    QStringList getColors() const;
    void setColors(QStringList aColors);

    int getDragPos() const;
    void setDragPos(int pos);

    Q_INVOKABLE void addColor(QColor);
    Q_INVOKABLE int indexOf(QColor) const;

    // QAbstractListModel
    QHash<int,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex&) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex&, int) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void colorsChanged();
    void dragPosChanged();

private:
    class Private;
    Private* iPrivate;
};

#endif // HARBOUR_COLOR_EDITOR_MODEL_H
