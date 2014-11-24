#include "keyEventFilter.h"
#include <QKeyEvent>
#include <iostream>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QFontMetrics>

KeyEventFilter::KeyEventFilter(QWidget *parent)
{
    _mainWindow = parent;

    _filterDialog = new QWidget(parent);
    _filterDialog->setStyleSheet("color:#38BFBF;background-color:#404040;");
    QHBoxLayout* horizontalLayout = new QHBoxLayout(_filterDialog);
    QSpacerItem* leftSpace = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _searchLabel = new QLabel(_filterDialog);
    QFont f( "Arial", 26, QFont::Bold);
    _searchLabel->setFont(f);
    QSpacerItem* rightSpace = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);

    horizontalLayout->addItem(leftSpace);
    horizontalLayout->addWidget(_searchLabel);
    horizontalLayout->addItem(rightSpace);

    _filterDialog->setLayout(horizontalLayout);
    _filterDialog->hide();

    _keyTimerDelay = 1000;
    _keyTimer = new QTimer(this);
    connect(_keyTimer, SIGNAL(timeout()), this, SLOT(endSearch()));
    _searchRunning = false;
    _searchString = QString("");

    connect(this, SIGNAL(searchChanged(QString)), parent, SLOT(runSearch(QString)));
}

KeyEventFilter::~KeyEventFilter()
{
    delete _keyTimer;
}

void KeyEventFilter::endSearch()
{
    _searchRunning = false;
    _filterDialog->hide();
}

bool KeyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);

        if(((key->key() >= Qt::Key_A) && (key->key() <= Qt::Key_Z)) || ((key->key() >= Qt::Key_0) && (key->key() <= Qt::Key_9)) || key->key() == Qt::Key_Space)
        {
            if (!_searchRunning)
            {
                _searchString.clear();
                _searchRunning = true;
            }
            _searchString.append(key->key());
            _keyTimer->start(_keyTimerDelay);
            _searchLabel->setText(_searchString);
            QRect boundingRect = _searchLabel->fontMetrics().boundingRect(_searchString);
            _filterDialog->setFixedSize(boundingRect.width() + 40, boundingRect.height() + 10);

            QPoint leftTopCorner((_mainWindow->size().width() - boundingRect.width()) / 2, (_mainWindow->size().height() - boundingRect.height()) / 2);
            _filterDialog->move(leftTopCorner);
            _filterDialog->show();

            emit searchChanged(_searchString);
        }
        else
        {
            return QObject::eventFilter(obj, event);
        }
        return true;
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }

    return false;
}
