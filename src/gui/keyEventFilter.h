#ifndef KEY_EVENT_FILTER_H
#define KEY_EVENT_FILTER_H

#include <QTimer>
#include <QWidget>

class KeyEventFilter : public QObject
{
    Q_OBJECT

public:
    KeyEventFilter(QWidget *parent);
    ~KeyEventFilter();

public slots:
    void endSearch();

signals:
    void searchChanged(const QString& searchString);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    int _keyTimerDelay;
    QTimer* _keyTimer;
    bool _searchRunning;
    QString _searchString;
};

#endif // KEY_EVENT_FILTER_H
