#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QEvent>
#include <QDebug>

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr)
        : QWidget(parent), existingFrame(nullptr)
    {
        // Create main buttons
        button1 = new QPushButton("Button 1", this);
        button2 = new QPushButton("Button 2", this);

        // Set fixed sizes for the buttons
        button1->setFixedSize(60, 30);
        button2->setFixedSize(60, 30);

        // Install event filters on the buttons
        button1->installEventFilter(this);
        button2->installEventFilter(this);

        // Layout for main buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(button1);
        buttonLayout->addWidget(button2);

        // Add a spacer to push buttons to the left
        buttonLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

        buttonLayout->setContentsMargins(0, 0, 0, 0); // No margins
        buttonLayout->setSpacing(0);                  // No spacing

        // TextEdit under the buttons
        QTextEdit *textEdit = new QTextEdit(this);
        textEdit->setPlaceholderText("Type here...");
        textEdit->setFixedHeight(100); // Adjust height as needed

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(textEdit);

        mainLayout->setContentsMargins(0, 0, 0, 0); // No margins
        mainLayout->setSpacing(0);                  // No spacing

        setLayout(mainLayout);
    }

protected:
    // Event filter to catch mouse enter and leave events
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::Enter)
        {
            handleMouseEnter(static_cast<QPushButton *>(obj));
        }
        else if (event->type() == QEvent::Leave)
        {
            handleMouseLeave(static_cast<QPushButton *>(obj));
        }
        return QWidget::eventFilter(obj, event); // Pass the event to the base class
    }

private:
    QPushButton *button1;
    QPushButton *button2;
    QFrame *existingFrame; // Keep track of the currently visible frame

    // Handle mouse enter
    void handleMouseEnter(QPushButton *button)
    {
        // Remove the existing frame if any
        if (existingFrame)
        {
            existingFrame->setParent(nullptr);
            existingFrame->deleteLater();
            existingFrame = nullptr;
        }

        // Create a new frame for the hovered button
        QFrame *newFrame = new QFrame(this);
        newFrame->setFrameShape(QFrame::NoFrame); // Ensure no default frame margins
        newFrame->setStyleSheet("QFrame { margin: 0px; padding: 0px; border: none; }");

        // Add unique buttons to the frame
        QVBoxLayout *frameLayout = new QVBoxLayout(newFrame);
        frameLayout->setContentsMargins(0, 0, 0, 0); // No margins
        frameLayout->setSpacing(10);                  // No spacing between buttons

        if (button == button1)
        {
            // Frame for Button 1
            QPushButton *frameButton1 = new QPushButton("Save", newFrame);
            QPushButton *frameButton2 = new QPushButton("Save", newFrame);
            QPushButton *frameButton3 = new QPushButton("Save", newFrame);


            // Set fixed sizes and remove internal padding using stylesheets
            // frameButton1->setFixedSize(120, 30);
            // frameButton2->setFixedSize(120, 30);

            // Set unique background colors for buttons
            frameButton1->setStyleSheet("QPushButton { margin: 0px; padding: 0px; border: none; text-align: left; background-color: lightblue; height: 30px; width: 120px;}");
            frameButton2->setStyleSheet("QPushButton { margin: 0px; padding: 0px; border: none; text-align: left; background-color: lightgreen; height: 30px; width: 120px;}");
            frameButton3->setStyleSheet("QPushButton { margin: 0px; padding: 0px; border: none; text-align: left; background-color: lightblue; height: 30px; width: 120px;}");

            frameLayout->addWidget(frameButton1);
            frameLayout->addWidget(frameButton2);
            frameLayout->addWidget(frameButton3);
        }
        else if (button == button2)
        {
            // Frame for Button 2
            QPushButton *frameButton1 = new QPushButton("Cut", newFrame);
            QPushButton *frameButton2 = new QPushButton("Copy", newFrame);
            QPushButton *frameButton3 = new QPushButton("Paste", newFrame);

            // Set fixed sizes and remove internal padding using stylesheets
            frameButton1->setFixedSize(120, 50);
            frameButton2->setFixedSize(120, 50);
            frameButton3->setFixedSize(120, 50);

            // Set unique background colors for buttons
            frameButton1->setStyleSheet("QPushButton { margin: 0px; padding: 0px; border: none; background-color: pink; }");
            frameButton2->setStyleSheet("QPushButton { margin: 0px; padding: 0px; border: none; background-color: yellow; }");
            frameButton3->setStyleSheet("QPushButton { margin: 0px; padding: 0px; border: none; background-color: orange; }");

            frameLayout->addWidget(frameButton1);
            frameLayout->addWidget(frameButton2);
            frameLayout->addWidget(frameButton3);
        }

        // Resize the frame and position it below the button
        newFrame->adjustSize();
        QPoint buttonBottomLeft = button->mapToParent(QPoint(0, button->height()-5));
        newFrame->move(buttonBottomLeft);

        // Save the new frame as the existing frame and make it visible
        newFrame->setVisible(true);
        existingFrame = newFrame;
    }

    // Handle mouse leave
    void handleMouseLeave(QPushButton *button)
    {
        Q_UNUSED(button); // No specific action required for leaving a button
        if (existingFrame)
        {
            // existingFrame->setVisible(false);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyWidget window;
    window.resize(400, 300); // Set initial size
    window.show();

    return app.exec();
}

#include "main.moc"
