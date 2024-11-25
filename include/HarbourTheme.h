/*
 * Copyright (C) 2018-2024 Slava Monich <slava@monich.com>
 * Copyright (C) 2018-2022 Jolla Ltd.
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

#ifndef HARBOUR_THEME_H
#define HARBOUR_THEME_H

#include <QtCore/QObject>
#include <QtGui/QColor>

class QQmlEngine;
class QJSEngine;

class HarbourTheme :
    public QObject
{
    Q_OBJECT
    Q_ENUMS(ColorScheme)
    Q_PROPERTY(ColorScheme colorScheme READ colorScheme NOTIFY colorSchemeChanged)
    Q_PROPERTY(QColor invertedPrimaryColor READ invertedPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QColor primaryColor READ primaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(bool lightOnDark READ lightOnDark NOTIFY colorSchemeChanged)
    Q_PROPERTY(bool darkOnLight READ darkOnLight NOTIFY colorSchemeChanged)

    Q_PROPERTY(qreal opacityFaint READ opacityFaint CONSTANT)
    Q_PROPERTY(qreal opacityLow READ opacityLow CONSTANT)
    Q_PROPERTY(qreal opacityHigh READ opacityHigh CONSTANT)
    Q_PROPERTY(qreal opacityOverlay READ opacityOverlay CONSTANT)

public:
    class Private;

    enum ColorScheme {
        LightOnDark,
        DarkOnLight
    };

    explicit HarbourTheme(QObject* aParent = Q_NULLPTR);

    ~HarbourTheme();

    // Callback for qmlRegisterSingletonType<HarbourTheme>
    static QObject* createSingleton(QQmlEngine*, QJSEngine*);

    QColor invertedPrimaryColor() const;
    QColor primaryColor() const;
    ColorScheme colorScheme() const;
    bool lightOnDark() const;
    bool darkOnLight() const;

    qreal opacityFaint() const;
    qreal opacityLow() const;
    qreal opacityHigh() const;
    qreal opacityOverlay() const;

    Q_INVOKABLE static QColor invertedColor(const QColor&);
    Q_INVOKABLE static qreal colorDifference(const QColor&, const QColor&);

Q_SIGNALS:
    void colorSchemeChanged();
    void primaryColorChanged();
};

#endif // HARBOUR_THEME_H
