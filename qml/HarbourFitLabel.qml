/*
 * Copyright (C) 2018 Jolla Ltd.
 * Copyright (C) 2018 Slava Monich <slava@monich.com>
 *
 * You may use this file under the terms of BSD license as follows:
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

import QtQuick 2.2
import Sailfish.Silica 1.0

Label {
    property int minFontSize: Theme.fontSizeTiny
    property int maxFontSize: Theme.fontSizeMedium
    property int refitting

    smooth: true
    visible: opacity > 0
    color: Theme.highlightColor
    anchors.margins: Theme.paddingMedium
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter

    Behavior on opacity { FadeAnimator {} }

    Component.onCompleted: refitText()

    onWidthChanged: refitText()
    onHeightChanged: refitText()
    onTextChanged: refitText()
    onMaxFontSizeChanged: refitText()
    onMinFontSizeChanged: refitText()

    function refitText() {
        refitting++
        if (refitting == 1 && paintedHeight > 0 && paintedWidth > 0) {
            if (font.pixelSize % 2) font.pixelSize++
            while (paintedWidth > width || paintedHeight > height) {
                if ((font.pixelSize -= 2) <= minFontSize)
                    break
            }
            while (paintedWidth < width && paintedHeight < height) {
                font.pixelSize += 2
            }
            font.pixelSize -= 2
            if (font.pixelSize >= maxFontSize) {
                font.pixelSize = maxFontSize
                return
            }
        }
        refitting--
    }
}