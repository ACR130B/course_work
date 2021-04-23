#ifndef VET_MAIN_WINDOW_H
#define VET_MAIN_WINDOW_H
#include <ui_main_window.h>
#include <QMainWindow>
#include <QtSql>

// Главный класс приложения, инкапсулирует всю логику работы
class MainWindow : public QMainWindow{
 Q_OBJECT
public:
	// Конструктор. Инициализирует графический интерфейс, начальные характеристики работы приложения и логику работы сигналов
	MainWindow(QWidget *parent = nullptr);
private:
	// Хранит объект класса для работы с графическим интерфейсом
	Ui_MainWindow gui;
	// Хранит указатель на модель таблицы
	QSqlQueryModel *model;
	// Хранит значение фильтра, которое принимается для фильтрации записей в таблицах
	QString lastFilter = "";
	// Хранит индекс рассматриваемого объекта (если создается новый объект = -1)
	int currentInd = -1;
	// Изменяет вывод данных в таблице
	void updateModel();
	// Демонстрирует данные выбранной записи журнала
	void showJournal();
	void showAnimal();
private slots:
	// Изменяет значение фильтра и запускает с ним поиск
	void changeFilter();
	// Создает новую запись журнала, либо новую карточку животного
	void newNote();
	// Сохранение записи в журнале
	void saveJournal();
	// Удаление записи в журнале
	void deleteJournal();
	// Сохраняет карточку животного
	void saveAnimal();
	// Удаляет карточку животного
	void deleteAnimal();
	// Обработка двойного нажатия на таблицы
	void doubleClickHandler(const QModelIndex &ind);
	// Обработка изменения вкладки
	void tabChangeHandler(int x);
};


#endif //VET_MAIN_WINDOW_H
