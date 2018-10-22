


#ifndef CONTROLABLETASKS_H
#define CONTROLABLETASKS_H


#include <QFuture>
#include <QRunnable>
#include <QThreadPool>
#include <QFutureInterfaceBase>


class TaskControl
{
public:
    TaskControl(QFutureInterfaceBase *f) : fu(f) {  }
    bool shouldRun() const { return !fu->isCanceled(); }
private:
    QFutureInterfaceBase *fu;
};

template <class T>
class ControllableTask
{
public:
    virtual ~ControllableTask() {}
    virtual T run(TaskControl& control) = 0;
};

template <typename T>
class RunControllableTask : public QFutureInterface<T> , public QRunnable
{
public:
    RunControllableTask(ControllableTask<T>* tsk) : task(tsk) { }
    virtual ~RunControllableTask() { delete task; }

    QFuture<T> start()
    {
        this->setRunnable(this);
        this->reportStarted();
        QFuture<T> future = this->future();
        QThreadPool::globalInstance()->start(this, /*m_priority*/ 0);
        return future;
    }

    void run()
    {
        if (this->isCanceled()) {
            this->reportCanceled();
            return;
        }
        TaskControl control(this);
        result = this->task->run(control);
        if (!this->isCanceled()) {
            this->reportResult(result);
        }
        this->reportFinished();
    }

    T  result;
    ControllableTask<T> *task;
};

class TaskExecutor {
public:
    template <class T>
    static QFuture<T> run(ControllableTask<T>* task) {
        return (new RunControllableTask<T>(task))->start();
    }

};


#endif // CONTROLABLETASKS_H


