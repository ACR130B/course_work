#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent) {
	gui.setupUi(this);

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("database.db");
	db.open();

	model = new QSqlQueryModel;
	showJournal();
	gui.tableView->setModel(model);
	updateModel();
	connect(gui.pushButton_3, SIGNAL(clicked()), this, SLOT(changeFilter()));
	connect(gui.pushButton_4, SIGNAL(clicked()), this, SLOT(deleteJournal()));
	connect(gui.pushButton_5, SIGNAL(clicked()), this, SLOT(saveJournal()));
	connect(gui.pushButton_6, SIGNAL(clicked()), this, SLOT(newNote()));
	connect(gui.pushButton_7, SIGNAL(clicked()), this, SLOT(deleteAnimal()));
	connect(gui.pushButton_8, SIGNAL(clicked()), this, SLOT(saveAnimal()));

	connect(gui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChangeHandler(int)));
	connect(gui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClickHandler(const QModelIndex &)));
}

void MainWindow::updateModel() {
	int currentTab = gui.tabWidget->currentIndex();
	QSqlQuery request;
	if (currentTab == 0) {
		request.prepare(
				R"(SELECT Journal.ID, Title as Услуга, Price as Цена, Time as Время, Date as Дата,
								Animals.Name as Питомец, Masters.Name as Хозяин FROM Journal
								LEFT JOIN Animals ON Journal.ID_Animal=Animals.ID
								LEFT JOIN Masters ON Masters.ID_Animal=Animals.ID
								WHERE Title like ?)");
	} else {
		request.prepare(
				R"(SELECT Animals.ID, Animals.Name as Кличка, Age as Возраст, Masters.Name as Хозяин FROM Animals
								LEFT JOIN Masters ON Masters.ID_Animal=Animals.ID
								WHERE Animals.Name like ?)");
	}
	request.addBindValue(QString("%1%").arg(lastFilter));
	request.exec();
	model->setQuery(request);
	gui.tableView->resizeColumnsToContents();
}

void MainWindow::changeFilter() {
	lastFilter = gui.lineEdit->text();
	updateModel();
}

void MainWindow::newNote() {
	currentInd = -1;
	gui.tabWidget->currentIndex() == 0 ? showJournal() : showAnimal();
}

void MainWindow::showJournal() {
	gui.comboBox->clear();
	QSqlQuery request(R"(SELECT ID FROM Animals)");
	while(request.next()) {
		gui.comboBox->addItem(request.value(0).toString());
	}
	if (currentInd == -1) {
		gui.label_2->setText("Новая запись");
		gui.lineEdit_2->setText("");
		gui.doubleSpinBox->setValue(0.0f);
		gui.spinBox->setValue(0);
		gui.timeEdit->setTime(QTime(0, 0));
		gui.dateEdit->setDate(QDate(2021, 01, 01));
		gui.pushButton_4->hide();
	} else {
		gui.label_2->setText(("Запись №"+std::to_string(currentInd)).c_str());
		request.prepare(R"(SELECT Title, Price, ID_Doctor, Time, Date, ID_Animal FROM Journal WHERE ID=?)");
		request.addBindValue(currentInd);
		request.exec();
		request.first();
		gui.lineEdit_2->setText(request.value(0).toString());
		gui.doubleSpinBox->setValue(request.value(1).toFloat());
		gui.spinBox->setValue(request.value(2).toInt());
		gui.timeEdit->setTime(request.value(3).toTime());
		gui.dateEdit->setDate(request.value(4).toDate());
		gui.comboBox->setCurrentText(request.value(5).toString());
		gui.pushButton_4->show();
	}
}

void MainWindow::saveJournal() {
	QSqlQuery request;
	if (currentInd == -1) {
		qDebug() << request.prepare(
				R"(INSERT INTO Journal (Title, Price, ID_Animal, ID_Doctor, Time, Date) VALUES (:title, :price, :animal, :doctor, :time, :date))");
	} else {
		request.prepare(
				R"(UPDATE Journal SET Title=:title, Price=:price, ID_Animal=:animal, ID_Doctor=:doctor, Time=:time, Date=:date WHERE ID=:id)");
		request.bindValue(":id", currentInd);
	}
	request.bindValue(":title", gui.lineEdit_2->text());
	request.bindValue(":price", gui.doubleSpinBox->value());
	request.bindValue(":animal", gui.comboBox->currentText().toInt());
	request.bindValue(":doctor", gui.spinBox->value());
	request.bindValue(":time", gui.timeEdit->time());
	request.bindValue(":date", gui.dateEdit->date());
	request.exec();
	updateModel();
	showJournal();
}

void MainWindow::deleteJournal() {
	QSqlQuery request;
	request.prepare(R"(DELETE FROM Journal WHERE ID=?)");
	request.addBindValue(currentInd);
	request.exec();
	updateModel();
	currentInd = -1;
	showJournal();
}

void MainWindow::showAnimal() {
	QSqlQuery request;
	if (currentInd == -1) {
		gui.label_11->setText("Новый питомец");
		gui.lineEdit_3->setText("");
		gui.lineEdit_5->setText("");
		gui.lineEdit_4->setText("");
		gui.spinBox_3->setValue(0);
		gui.textEdit->setText("");
		gui.label_15->hide();
		gui.pushButton_7->hide();
	} else {
		gui.label_11->setText(("Питомец №"+std::to_string(currentInd)).c_str());
		request.prepare(R"(
			SELECT Masters.Name, Phone, Animals.Name, Age, History FROM Animals
			LEFT JOIN Masters ON Masters.ID_Animal=Animals.ID
			WHERE Animals.ID=?)");
		request.addBindValue(currentInd);
		request.exec();
		request.first();
		gui.lineEdit_3->setText(request.value(0).toString());
		gui.lineEdit_5->setText(request.value(1).toString());
		gui.lineEdit_4->setText(request.value(2).toString());
		gui.spinBox_3->setValue(request.value(3).toInt());
		gui.textEdit->setText(request.value(4).toString());
		request.prepare(R"(SELECT COUNT(*) FROM Journal WHERE ID_Animal=?)");
		request.addBindValue(currentInd);
		request.exec();
		qDebug()<<request.lastError();
		request.first();
		gui.label_15->setText("Количество приемов: "+request.value(0).toString());
		gui.label_15->show();
		gui.pushButton_7->show();
	}
}

void MainWindow::saveAnimal() {
	QSqlQuery request;
	if (currentInd == -1) {
		request.prepare(R"(INSERT INTO Animals(Name, Age, History) VALUES (:name, :age, :history))");
	} else {
		request.prepare(R"(UPDATE Animals SET Name=:name, Age=:age, History=:history WHERE ID=:id)");
		request.bindValue(":id", currentInd);
	}
	request.bindValue(":name", gui.lineEdit_4->text());
	request.bindValue(":age", gui.spinBox_3->value());
	request.bindValue(":history", gui.textEdit->toPlainText());
	request.exec();
	if (currentInd == -1) {
		int animalId = request.lastInsertId().toInt();
		request.prepare(R"(INSERT INTO Masters(ID_Animal, Name, Phone) VALUES (:id, :name, :phone))");
		request.bindValue(":id", animalId);
	} else {
		request.prepare(R"(UPDATE Masters SET Name=:name, Phone=:phone WHERE ID_Animal=:id)");
		request.bindValue(":id", currentInd);
	}
	request.bindValue(":name", gui.lineEdit_3->text());
	request.bindValue(":phone", gui.lineEdit_5->text());
	request.exec();
	updateModel();
	showAnimal();
}

void MainWindow::deleteAnimal() {
	QSqlQuery request;
	request.prepare(R"(DELETE FROM Animals WHERE ID=?)");
	request.addBindValue(currentInd);
	request.exec();
	request.prepare(R"(DELETE FROM Masters WHERE ID_Animal=?)");
	request.addBindValue(currentInd);
	request.exec();
	updateModel();
	currentInd = -1;
	showAnimal();
}

void MainWindow::doubleClickHandler(const QModelIndex &ind) {
currentInd = gui.tableView->model()->data(gui.tableView->model()->index(ind.row(), 0)).toInt();
if (gui.tabWidget->currentIndex() == 0) {
	showJournal();
} else {
	showAnimal();
}
}

void MainWindow::tabChangeHandler(int x) {
	currentInd = -1;
	x == 0 ? showJournal() : showAnimal();
	updateModel();
}


