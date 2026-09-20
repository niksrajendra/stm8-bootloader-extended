#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>  // Required for the file dialog
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "stm8_bootloader_utility_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    // 1. Create a generic container widget
    QWidget *centralWidget = new QWidget(&w);

    // 2. Assign your vertical layout to this container widget
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 3. Create and add your child widgets to the layout
    QPushButton *fileButton = new QPushButton("Choose Firmware File...", centralWidget);
    QPushButton *button1 = new QPushButton("Top Button", centralWidget);
    QPushButton *button2 = new QPushButton("Middle Button", centralWidget);
    QPushButton *button3 = new QPushButton("Bottom Button", centralWidget);

    layout->addWidget(fileButton);
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    // 4. Handle the file chooser click event using a Lambda function
    QObject::connect(fileButton, &QPushButton::clicked, centralWidget, [centralWidget]() {
        QString filePath = QFileDialog::getOpenFileName(
            centralWidget,
            "Select STM8 Hex/Bin File",  // Dialog title
            "",                          // Default directory (empty means current)
            "Hex Files (*.hex);;Binary Files (*.bin);;All Files (*)" // File filters
            );

        if (!filePath.isEmpty()) {
            qDebug() << "Selected file:" << filePath;
            // You can now pass 'filePath' to your flashing/bootloader logic
        }
    });

    // 5. Set the container as the central widget of your MainWindow
    w.setCentralWidget(centralWidget);
    w.show();
    return QApplication::exec();
}
