/*
 * Copyright 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "controlpanel.h"
#include "outputconfig.h"
#include "unifiedoutputconfig.h"

#include <QVBoxLayout>

#include <kscreen/config.h>

ControlPanel::ControlPanel(QWidget *parent)
    : QScrollArea(parent)
    , mUnifiedOutputCfg(0)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(true);

    QWidget *widget = new QWidget(this);
    mLayout = new QVBoxLayout(widget);
    mLayout->setSizeConstraint(QLayout::QLayout::SetMinAndMaxSize);

    setWidget(widget);
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::setConfig(const KScreen::ConfigPtr &config)
{
    qDeleteAll(mOutputConfigs);
    mOutputConfigs.clear();

    mConfig = config;
    if (mUnifiedOutputCfg) {
        delete mUnifiedOutputCfg;
        mUnifiedOutputCfg = 0;
    }

    Q_FOREACH (const KScreen::OutputPtr &output, mConfig->outputs()) {
        OutputConfig *outputCfg = new OutputConfig(output, widget());
        outputCfg->hide();
        connect(outputCfg, &OutputConfig::changed,
                this, &ControlPanel::changed);

        mLayout->addWidget(outputCfg);
        mOutputConfigs << outputCfg;
    }
}

void ControlPanel::activateOutput(const KScreen::OutputPtr &output)
{
    // Ignore activateOutput when in unified mode
    if (mUnifiedOutputCfg) {
        return;
    }

    Q_FOREACH (OutputConfig *cfg, mOutputConfigs) {
        cfg->setVisible(cfg->output()->id() == output->id());
    }
}

void ControlPanel::setUnifiedOutput(const KScreen::OutputPtr &output)
{
    Q_FOREACH (OutputConfig *config, mOutputConfigs) {
        if (!config->output()->isConnected()) {
            continue;
        }

        config->setVisible(output == 0);
    }

    if (output.isNull()) {
        mUnifiedOutputCfg->deleteLater();
        mUnifiedOutputCfg = 0;
    } else {
        mUnifiedOutputCfg = new UnifiedOutputConfig(mConfig, widget());
        mUnifiedOutputCfg->setOutput(output);
        mUnifiedOutputCfg->setVisible(true);
        mLayout->insertWidget(mLayout->count() - 2, mUnifiedOutputCfg);
        connect(mUnifiedOutputCfg, &UnifiedOutputConfig::changed,
                this, &ControlPanel::changed);
    }
}
