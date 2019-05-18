/*
 * Copyright (C) 2019 Jolla Ltd.
 * Copyright (C) 2019 Slava Monich <slava.monich@jolla.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3. Neither the names of the copyright holders nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HARBOUR_QRCODE_GENERATOR_H
#define HARBOUR_QRCODE_GENERATOR_H

#include <QtQml>

class HarbourQrCodeGenerator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString code READ code NOTIFY codeChanged)
    Q_PROPERTY(QString qrcode READ code NOTIFY codeChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    HarbourQrCodeGenerator(QObject* aParent = Q_NULLPTR);

    QString text() const;
    void setText(QString aValue);

    QString code() const;
    bool running() const;

    Q_INVOKABLE static QByteArray generate(QString aText);

    // Callback for qmlRegisterSingletonType<HarbourQrCodeGenerator>
    static QObject* createSingleton(QQmlEngine* aEngine, QJSEngine* aScript);

Q_SIGNALS:
    void textChanged();
    void codeChanged();
    void runningChanged();

private:
    class Task;
    class Private;
    Private* iPrivate;
};

QML_DECLARE_TYPE(HarbourQrCodeGenerator)

#endif // HARBOUR_QRCODE_GENERATOR_H
