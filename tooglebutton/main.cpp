#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>

class ToggleSwitch : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    explicit ToggleSwitch(QWidget *parent = nullptr)
        : QWidget(parent), m_checked(false), m_offset(0.0) {
        setFixedSize(60, 30);

        // Default colors
        m_backgroundColor = QColor("#CCCCCC");

        // Animation for the slider movement
        m_offsetAnimation = new QPropertyAnimation(this, "offset", this);
        m_offsetAnimation->setDuration(300); // 300ms
        m_offsetAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        // Animation for the background color change
        m_colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
        m_colorAnimation->setDuration(300); // 300ms
        m_colorAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    }

    ~ToggleSwitch() {
        delete m_offsetAnimation;
        delete m_colorAnimation;
    }

    bool isChecked() const { return m_checked; }

    qreal offset() const { return m_offset; }
    void setOffset(qreal offset) {
        if (m_offset != offset) {
            m_offset = offset;
            emit offsetChanged();
            update(); // Repaint the widget
        }
    }

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color) {
        if (m_backgroundColor != color) {
            m_backgroundColor = color;
            emit backgroundColorChanged();
            update(); // Repaint the widget
        }
    }

signals:
    void toggled(bool checked);
    void offsetChanged();
    void backgroundColorChanged();

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Background
        QRectF backgroundRect(0, height()*1/4, width(), height()/2);
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0.0, m_backgroundColor.lighter(120));
        gradient.setColorAt(1.0, m_backgroundColor);
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(backgroundRect, height() / 4, height() / 4);

        // Toggle
        qreal toggleDiameter = height() * 0.8;
        qreal toggleX = m_offset * (width() - toggleDiameter);
        qreal toggleY = (height() - toggleDiameter) / 2;
        QRectF toggleRect(toggleX, toggleY, toggleDiameter, toggleDiameter);
        painter.setBrush(QColor("#FFFFFF"));
        painter.drawEllipse(toggleRect);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_checked = !m_checked;
            emit toggled(m_checked);

            // Start the toggle movement animation
            m_offsetAnimation->setStartValue(m_offset);
            m_offsetAnimation->setEndValue(m_checked ? 1.0 : 0.0);
            m_offsetAnimation->start();

            // Start the color animation
            m_colorAnimation->setStartValue(m_backgroundColor);
            m_colorAnimation->setEndValue(m_checked ? QColor("#4CAF50") : QColor("#CCCCCC"));
            m_colorAnimation->start();
        }
    }

private:
    bool m_checked;
    qreal m_offset; // 0.0 (left) to 1.0 (right)
    QColor m_backgroundColor;
    QPropertyAnimation *m_offsetAnimation;
    QPropertyAnimation *m_colorAnimation;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(200, 100);

    ToggleSwitch *toggle = new ToggleSwitch(&window);
    toggle->move(70, 35);
    QObject::connect(toggle, &ToggleSwitch::toggled, [](bool checked) {
        qDebug() << "Toggled:" << (checked ? "ON" : "OFF");
    });

    window.show();
    return app.exec();
}

#include "main.moc"
