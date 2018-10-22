#include <QCoreApplication>
#include <QDebug>
#include <QFuture>
#include <QtConcurrent>
#include <QThread>

int main(int argc, char *argv[])
{

    QCoreApplication app(argc,argv);

    qDebug() << "Hello World !!!\n";
    QFuture<void> f1 = QtConcurrent::run([](){
            QThread::currentThread()->sleep(5);
            qDebug() << "Hello PAPA !!!\n";
            QCoreApplication::exit();
        }
    );
    QFuture<void> f2 = QtConcurrent::run([](){qDebug() << "Hello MOM !!!\n";});
    f1.waitForFinished();
    f2.waitForFinished();

    return app.exec();
}
