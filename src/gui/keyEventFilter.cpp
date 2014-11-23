#include "keyEventFilter.h"
#include <QKeyEvent>
#include <iostream>

KeyEventFilter::KeyEventFilter(QWidget *parent)
{
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
            std::cerr << _searchString.toStdString();
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
