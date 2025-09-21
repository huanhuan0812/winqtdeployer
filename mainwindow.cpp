#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QProcess>
#include <QMessageBox>
#include <QClipboard>
#include <QGuiApplication>
#include <QStyle>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QScrollArea>
#include <QApplication>
#include <QStatusBar>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), settings(new QSettings("MyCompany", "WindDeployGUI", this))
{
    QString systemLanguage = QLocale::system().name(); // 格式如: "zh_CN", "en_US"

    // 如果系统语言不在支持的语言列表中，使用默认英语
    QSet<QString> supportedLanguages = {"en_US", "zh_CN"};
    if (!supportedLanguages.contains(systemLanguage)) {
        systemLanguage = "en_US";
    }

    currentLanguage = systemLanguage; // 设置当前语言为系统语言
    //currentLanguage = "en_US"; // 默认语言
    loadTranslations();
    setupUI();
    createConnections();
    loadSettings();
    setWindowTitle(translate("title"));
    resize(900, 600);
}

MainWindow::~MainWindow() {
}

QString MainWindow::translate(const QString &key) {
    // 首选：当前语言的翻译
    if (translationMap.contains(key)) {
        QString translation = translationMap.value(key);
        if (!translation.isEmpty()) {
            return translation;
        }
    }
    // 备用：英语翻译（如果当前不是英语）
    if (currentLanguage != "en_US") {
        // 这里可以添加逻辑来加载英语翻译作为回退
        // 或者维护一个默认的英语翻译映射
    }
    // 最后回退：返回键本身
    qWarning() << "Translation not found for key:" << key;
    return key;
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 文件路径选择组
    QGroupBox *fileGroup = new QGroupBox(translate("filepath"), this);
    QFormLayout *fileLayout = new QFormLayout(fileGroup);

    executableEdit = new QLineEdit(this);
    browseExecutableBtn = new QPushButton(translate("browse"), this);
    QHBoxLayout *exeLayout = new QHBoxLayout();
    exeLayout->addWidget(executableEdit);
    exeLayout->addWidget(browseExecutableBtn);

    qmlDirEdit = new QLineEdit(this);
    browseQmlDirBtn = new QPushButton(translate("browse"), this);
    QHBoxLayout *qmlLayout = new QHBoxLayout();
    qmlLayout->addWidget(qmlDirEdit);
    qmlLayout->addWidget(browseQmlDirBtn);

    fileLayout->addRow(translate("executable"), exeLayout);
    fileLayout->addRow(translate("qmldictionary"), qmlLayout);

    // 模块选择组
    QGroupBox *moduleGroup = new QGroupBox(translate("modleselection"), this);
    QVBoxLayout *moduleLayout = new QVBoxLayout(moduleGroup);

    selectModulesBtn = new QPushButton(translate("selectmodels_btn"), this);
    selectModulesBtn->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));

    selectedModulesDisplay = new QTextEdit(this);
    selectedModulesDisplay->setMaximumHeight(80);
    selectedModulesDisplay->setReadOnly(true);
    selectedModulesDisplay->setPlaceholderText(translate("none_selected"));

    moduleLayout->addWidget(selectModulesBtn);
    moduleLayout->addWidget(new QLabel(translate("selected_to_exclude")));
    moduleLayout->addWidget(selectedModulesDisplay);

    // 选项组
    QGroupBox *optionsGroup = new QGroupBox(translate("deployoptions"), this);
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    noSystemD3dCompilerCheck = new QCheckBox(translate("no_d3d_compiler"), this);
    noSystemDxcCompilerCheck = new QCheckBox(translate("no_dxc_compiler"), this);
    noCompilerRuntimeCheck = new QCheckBox(translate("no_compiler_runtime"), this);
    noTranslationsCheck = new QCheckBox(translate("no_translations"), this);
    noQuickImportCheck = new QCheckBox(translate("no_quick_import"), this);
    noOpenGlSwCheck = new QCheckBox(translate("no_opengl_sw"), this);
    noFfmpegCheck = new QCheckBox(translate("no_ffmpeg"), this);
    forceCheck = new QCheckBox(translate("force"), this);
    autoRunCheck = new QCheckBox(translate("auto_run"), this);

    // 设置默认选中状态
    noSystemD3dCompilerCheck->setChecked(true);
    noSystemDxcCompilerCheck->setChecked(true);
    noCompilerRuntimeCheck->setChecked(false);
    noTranslationsCheck->setChecked(true);
    noQuickImportCheck->setChecked(false);
    noOpenGlSwCheck->setChecked(false);
    noFfmpegCheck->setChecked(false);
    forceCheck->setChecked(false);
    autoRunCheck->setChecked(true);

    optionsLayout->addWidget(noSystemD3dCompilerCheck);
    optionsLayout->addWidget(noSystemDxcCompilerCheck);
    optionsLayout->addWidget(noCompilerRuntimeCheck);
    optionsLayout->addWidget(noTranslationsCheck);
    optionsLayout->addWidget(noQuickImportCheck);
    optionsLayout->addWidget(noOpenGlSwCheck);
    optionsLayout->addWidget(noFfmpegCheck);
    optionsLayout->addWidget(forceCheck);
    optionsLayout->addWidget(autoRunCheck);

    // 按钮组
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    generateBtn = new QPushButton(translate("auto_run"), this);
    copyBtn = new QPushButton(translate("copy_command"), this);
    runBtn = new QPushButton(translate("run_only"), this);

    generateBtn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    copyBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    runBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    buttonLayout->addWidget(generateBtn);
    buttonLayout->addWidget(copyBtn);
    buttonLayout->addWidget(runBtn);

    // 输出组
    QGroupBox *outputGroup = new QGroupBox(translate("generated_command"), this);
    QVBoxLayout *outputLayout = new QVBoxLayout(outputGroup);
    commandOutput = new QTextEdit(this);
    commandOutput->setPlaceholderText(translate("command_placeholder"));
    commandOutput->setMaximumHeight(100);
    outputLayout->addWidget(commandOutput);

    // 状态栏
    statusLabel = new QLabel(translate("ready"), this);
    statusBar()->addWidget(statusLabel);

    // 组装主布局
    mainLayout->addWidget(fileGroup);
    mainLayout->addWidget(moduleGroup);
    mainLayout->addWidget(optionsGroup);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(outputGroup);

    setCentralWidget(centralWidget);
}

void MainWindow::createConnections()
{
    connect(browseExecutableBtn, &QPushButton::clicked, this, &MainWindow::browseExecutable);
    connect(browseQmlDirBtn, &QPushButton::clicked, this, &MainWindow::browseQmlDir);
    connect(selectModulesBtn, &QPushButton::clicked, this, &MainWindow::selectModules);
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generateCommand);
    connect(copyBtn, &QPushButton::clicked, this, &MainWindow::copyCommand);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runCommand);
    connect(executableEdit, &QLineEdit::textChanged, this, &MainWindow::onExecutableChanged);
}

void MainWindow::loadSettings()
{
    settings->beginGroup("Paths");
    executableEdit->setText(settings->value("lastExecutable").toString());
    qmlDirEdit->setText(settings->value("lastQmlDir").toString());
    settings->endGroup();

    settings->beginGroup("Modules");
    QStringList modules = settings->value("selectedModules").toStringList();
    selectedModules = QSet<QString>(modules.begin(), modules.end());
    updateModulesDisplay();
    settings->endGroup();

    settings->beginGroup("PluginTypes");
    QStringList pluginTypes = settings->value("selectedPluginTypes").toStringList();
    selectedPluginTypes = QSet<QString>(pluginTypes.begin(), pluginTypes.end());
    settings->endGroup();
}

void MainWindow::saveSettings()
{
    settings->beginGroup("Paths");
    settings->setValue("lastExecutable", executableEdit->text());
    settings->setValue("lastQmlDir", qmlDirEdit->text());
    settings->endGroup();

    settings->beginGroup("Modules");
    settings->setValue("selectedModules", QStringList(selectedModules.values()));
    settings->endGroup();

    settings->beginGroup("PluginTypes");
    settings->setValue("selectedPluginTypes", QStringList(selectedPluginTypes.values()));
    settings->endGroup();
}

void MainWindow::updateModulesDisplay()
{
    if (selectedModules.isEmpty() && selectedPluginTypes.isEmpty()) {
        selectedModulesDisplay->setPlainText(translate("none_selected"));
    } else {
        // 创建友好的显示名称映射
        QMap<QString, QString> displayNames = {
            {"quick-import", "Qt Quick 1导入"},
            {"angle", "ANGLE库"},
            {"qml-debug", "QML调试库"},
            {"3danimation", "3D动画模块"},
            {"3dcore", "3D核心模块"},
            {"3dextras", "3D额外模块"},
            {"3dinput", "3D输入模块"},
            {"3dlogic", "3D逻辑模块"},
            {"3dquick", "3D Quick模块"},
            {"3dquickanimation", "3D Quick动画"},
            {"3dquickextras", "3D Quick额外"},
            {"3dquickinput", "3D Quick输入"},
            {"3dquickrender", "3D Quick渲染"},
            {"3dquickscene2d", "3D Quick场景2D"},
            {"3drender", "3D渲染模块"},
            {"activeqt", "ActiveQt模块"},
            {"bluetooth", "蓝牙模块"},
            {"charts", "图表模块"},
            {"concurrent", "并发模块"},
            {"core", "核心模块"},
            {"core5compat", "Core5兼容模块"},
            {"datavisualization", "数据可视化"},
            {"dbus", "DBus模块"},
            {"gui", "GUI模块"},
            {"network", "网络模块"},
            {"opengl", "OpenGL模块"},
            {"positioning", "定位模块"},
            {"printsupport", "打印支持"},
            {"qml", "QML模块"},
            {"quick", "Quick模块"},
            {"quickcontrols2", "Quick Controls 2"},
            {"quickwidgets", "Quick Widgets"},
            {"sql", "SQL模块"},
            {"svg", "SVG模块"},
            {"test", "测试模块"},
            {"webchannel", "Web通道模块"},
            {"websockets", "WebSocket模块"},
            {"widgets", "Widgets模块"},
            {"xml", "XML模块"},
            {"audio", "音频插件"},
            {"bearer", "网络承载插件"},
            {"generic", "通用插件"},
            {"geometryloaders", "几何加载器插件"},
            {"geoservices", "地理服务插件"},
            {"iconengines", "图标引擎插件"},
            {"imageformats", "图像格式插件"},
            {"mediaservice", "媒体服务插件"},
            {"platforms", "平台插件"},
            {"playlistformats", "播放列表格式插件"},
            {"position", "位置插件"},
            {"printsupport", "打印支持插件"},
            {"renderers", "渲染器插件"},
            {"renderplugins", "渲染插件"},
            {"sceneparsers", "场景解析器插件"},
            {"sensorgestures", "传感器手势插件"},
            {"sensors", "传感器插件"},
            {"sqldrivers", "SQL驱动插件"},
            {"styles", "样式插件"},
            {"texttospeech", "文本转语音插件"},
            {"webview", "WebView插件"}
        };

        QStringList displayList;

        // 添加核心和库选项
        if (!selectedModules.isEmpty()) {
            QStringList sortedModules = selectedModules.values();
            sortedModules.sort();
            for (const QString &module : sortedModules) {
                QString displayName = displayNames.value(module, module);
                displayList.append(QString("• --no-%1 (%2)").arg(module).arg(displayName));
            }
        }

        // 添加插件类型选项
        if (!selectedPluginTypes.isEmpty()) {
            QStringList sortedPluginTypes = selectedPluginTypes.values();
            sortedPluginTypes.sort();
            for (const QString &pluginType : sortedPluginTypes) {
                QString displayName = displayNames.value(pluginType, pluginType);
                displayList.append(QString("• --skip-plugin-types %1 (%2)").arg(pluginType).arg(displayName));
            }
        }

        selectedModulesDisplay->setPlainText(displayList.join("\n"));
    }
}

bool MainWindow::loadTranslations() {
    QFile file(":/translations.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open translations file:" << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return false;
    }

    QJsonObject rootObj = doc.object();
    QString lang = currentLanguage.isEmpty() ? "en_US" : currentLanguage;

    if (!rootObj.contains(lang)) {
        qWarning() << "Language not found:" << lang;
        return false;
    }

    QJsonObject langObj = rootObj[lang].toObject();
    translationMap.clear();

    for (auto it = langObj.constBegin(); it != langObj.constEnd(); ++it) {
        translationMap[it.key()] = it.value().toString();
    }

    return true;
}


void MainWindow::selectModules()
{
    ModuleSelectionDialog dialog(this);

    // 设置之前选择的模块
    QListWidget *moduleList = dialog.findChild<QListWidget*>();
    if (moduleList) {
        for (int i = 0; i < moduleList->count(); ++i) {
            QListWidgetItem *item = moduleList->item(i);
            if (selectedModules.contains(item->data(Qt::UserRole).toString())) {
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }

    if (dialog.exec() == QDialog::Accepted) {
        selectedModules = dialog.getSelectedModules();
        updateModulesDisplay();
        saveSettings();
    }
}

void MainWindow::browseExecutable()
{
    QString file = QFileDialog::getOpenFileName(this, translate("select_executable"), "", "Executable Files (*.exe)");
    if (!file.isEmpty()) {
        executableEdit->setText(file);
    }
}

void MainWindow::browseQmlDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, translate("select_qml_dir"));
    if (!dir.isEmpty()) {
        qmlDirEdit->setText(dir);
    }
}

void MainWindow::onExecutableChanged(const QString &path)
{
    // 当可执行文件路径改变时，可以做一些预处理
}

QSet<QString> MainWindow::getDirectoryContents(const QString &dirPath)
{
    QSet<QString> contents;
    QDir dir(dirPath);

    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : files) {
        contents.insert(fileInfo.fileName());
    }

    return contents;
}

void MainWindow::showDirectoryContents(const QSet<QString> &contents)
{
    QString message = translate("directory_content") + ":\n\n";

    if (contents.isEmpty()) {
        message += translate("directory_empty");
    } else {
        for (const QString &item : contents) {
            message += "• " + item + "\n";
        }
    }

    message += "\n" + translate("select_opration");
    QMessageBox::information(this, translate("directory_content"), message);
}

bool MainWindow::checkAndCleanDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    QFileInfo exeInfo(executableEdit->text());
    QString exeName = exeInfo.fileName();

    // 获取目录内容（排除.exe本身）
    QSet<QString> contents = getDirectoryContents(dirPath);
    contents.remove(exeName);

    // 如果目录中只有.exe文件，直接返回true
    if (contents.isEmpty()) {
        return true;
    }

    // 显示目录内容
    showDirectoryContents(contents);

    // 询问用户操作
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(translate("directory_cleanup"));
    msgBox.setText(translate("dir_has_content"));
    msgBox.setIcon(QMessageBox::Question);

    QPushButton *clearButton = msgBox.addButton(translate("clear_all"), QMessageBox::DestructiveRole);
    QPushButton *moveButton = msgBox.addButton(translate("move_to_temp"), QMessageBox::ActionRole);
    QPushButton *cancelButton = msgBox.addButton(translate("cancel"), QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == clearButton) {
        return clearDirectory(dirPath, executableEdit->text());
    } else if (msgBox.clickedButton() == moveButton) {
        return moveToTempDirectory(dirPath, executableEdit->text());
    } else {
        return false; // 用户取消
    }
}

bool MainWindow::clearDirectory(const QString &dirPath, const QString &exePath)
{
    QDir dir(dirPath);
    QFileInfo exeInfo(exePath);
    QString exeName = exeInfo.fileName();

    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    bool success = true;
    for (const QFileInfo &fileInfo : files) {
        if (fileInfo.fileName() != exeName) {
            if (fileInfo.isDir()) {
                QDir subDir(fileInfo.absoluteFilePath());
                if (!subDir.removeRecursively()) {
                    success = false;
                    QMessageBox::warning(this, translate("warning"),
                        QString(translate("unable_remove_dir")+fileInfo.fileName()));
                }
            } else {
                if (!QFile::remove(fileInfo.absoluteFilePath())) {
                    success = false;
                    QMessageBox::warning(this, translate("warning"),
                        QString(translate("unable_remove_file")+fileInfo.fileName()));
                }
            }
        }
    }

    if (success) {
        QMessageBox::information(this, translate("success"), translate("cleanup_success"));
    }

    return success;
}

bool MainWindow::moveToTempDirectory(const QString &sourceDir, const QString &exePath)
{
    QDir dir(sourceDir);
    QFileInfo exeInfo(exePath);
    QString exeName = exeInfo.fileName();

    // 创建临时目录
    QString tempDirName = QString("backup_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString tempDirPath = QDir::tempPath() + "/" + tempDirName;

    if (!dir.mkpath(tempDirPath)) {
        QMessageBox::critical(this, translate("error"), translate("tempdir_unable"));
        return false;
    }

    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    bool success = true;
    for (const QFileInfo &fileInfo : files) {
        if (fileInfo.fileName() != exeName) {
            QString destPath = tempDirPath + "/" + fileInfo.fileName();
            if (fileInfo.isDir()) {
                QDir subDir(fileInfo.absoluteFilePath());
                if (!subDir.rename(fileInfo.absoluteFilePath(), destPath)) {
                    success = false;
                    QMessageBox::warning(this, translate("warning"),
                        QString(translate("unable_move_dir")+fileInfo.fileName()));
                }
            } else {
                if (!QFile::rename(fileInfo.absoluteFilePath(), destPath)) {
                    success = false;
                    QMessageBox::warning(this, translate("warning"),
                        QString(translate("unable_move_file")+fileInfo.fileName()));
                }
            }
        }
    }

    if (success) {
        QMessageBox::information(this, translate("success"),
            QString("文件已移动到临时目录:\n%1").arg(tempDirPath));
    }

    return success;
}

QString MainWindow::generateCommandString()
{
    QString command = "windeployqt";

    // 添加可执行文件路径
    QString exePath = executableEdit->text().trimmed();
    if (exePath.isEmpty()) {
        QMessageBox::warning(this, translate("warning"), translate("select_executable"));
        return "";
    }

    QFileInfo exeInfo(exePath);
    if (!exeInfo.exists()) {
        QMessageBox::warning(this, translate("warning"), translate("file_not_exist"));
        return "";
    }

    // 检查并清理目录
    QString exeDir = exeInfo.absolutePath();
    if (!checkAndCleanDirectory(exeDir)) {
        QMessageBox::information(this, translate("info"), translate("operation_cancelled"));
        return "";
    }

    command += " \"" + exePath + "\"";

    // 添加QML目录
    QString qmlDir = qmlDirEdit->text().trimmed();
    if (!qmlDir.isEmpty() && QDir(qmlDir).exists()) {
        command += " --qmldir \"" + qmlDir + "\"";
    }

    // 添加默认的lib和plugin目录
    //command += " --libdir \"" + exeDir + "/lib\"";
    //command += " --plugindir \"" + exeDir + "/plugin\"";
    //command += " --libdir lib";
    command += " --plugindir plugins";

    if (!selectedSpecificPlugins.isEmpty()) {
        QStringList sortedPlugins = selectedSpecificPlugins.values();
        sortedPlugins.sort();
        command += " --exclude-plugins " + sortedPlugins.join(",");
    }

    // 添加排除模块参数
    if (!selectedModules.isEmpty()) {
        QStringList sortedModules = selectedModules.values();
        sortedModules.sort();
        for (const QString &module : sortedModules) {
            command += " --no-" + module;
        }
    }

    // 添加排除插件类型参数
    if (!selectedPluginTypes.isEmpty()) {
        QStringList sortedPluginTypes = selectedPluginTypes.values();
        sortedPluginTypes.sort();
        command += " --skip-plugin-types " + sortedPluginTypes.join(",");
    }

    // 添加选项
    if (noSystemD3dCompilerCheck->isChecked()) command += " --no-system-d3d-compiler";
    if (noSystemDxcCompilerCheck->isChecked()) command += " --no-system-dxc-compiler";
    if (noCompilerRuntimeCheck->isChecked()) command += " --no-compiler-runtime";
    if (noTranslationsCheck->isChecked()) command += " --no-translations";
    if (noQuickImportCheck->isChecked()) command += " --no-quick-import";
    if (noOpenGlSwCheck->isChecked()) command += " --no-opengl-sw";
    if (noFfmpegCheck->isChecked()) command += " --no-ffmpeg";
    if (forceCheck->isChecked()) command += " --force";

    return command;
}

void MainWindow::generateCommand()
{
    QString command = generateCommandString();
    if (command.isEmpty()) {
        return;
    }

    commandOutput->setPlainText(command);
    statusLabel->setText(translate("command_generated"));

    // 如果设置了自动运行，则直接运行命令
    if (autoRunCheck->isChecked()) {
        runCommand();
    }
    
    saveSettings();
}

void MainWindow::copyCommand()
{
    QString command = commandOutput->toPlainText();
    if (!command.isEmpty()) {
        QGuiApplication::clipboard()->setText(command);
        statusLabel->setText(translate("command_copied"));
    } else {
        QMessageBox::information(this, translate("info"), "没有可复制的命令内容");
    }
}

bool MainWindow::createFolder(const QString &path)
{
    QDir dir;
    if (dir.mkpath(path)) {
        qDebug() << "Folder created:" << path;
        return true;
    } else {
        qDebug() << "Failed to create folder:" << path;
        return false;
    }
}

void MainWindow::runCommand()
{

    QString command;
    
    // 如果输出框为空，则生成新命令
    if (commandOutput->toPlainText().isEmpty()) {
        command = generateCommandString();
        if (command.isEmpty()) {
            return;
        }
        commandOutput->setPlainText(command);
    } else {
        command = commandOutput->toPlainText();
    }

    statusLabel->setText(translate("executing"));
    QApplication::processEvents(); // 更新UI

    QProcess process;
    QFileInfo exeInfo(executableEdit->text());

    // 确保路径使用正确的分隔符
    QString workingDir = exeInfo.absolutePath();
    workingDir = QDir::toNativeSeparators(workingDir); // 转换为本地分隔符
    process.setWorkingDirectory(workingDir);

    //if (!createFolder(workingDir+"/lib"))QMessageBox::warning(this, translate("warning"),
    //        QString(translate("unable_create_lib_dir")+"\n%1/lib").arg(workingDir));
    if (!createFolder(workingDir+"/plugins"))QMessageBox::warning(this, translate("warning"),
            QString(translate("unable_create_plugins_dir")+"\n%1/plugins").arg(workingDir));

    // 调试信息 - 显示实际执行的命令和工作目录
    qDebug() << "Working directory:" << workingDir;
    qDebug() << "Command:" << command;

    // 在Windows上使用cmd执行命令
    #ifdef Q_OS_WIN
    // 使用完整的命令路径，避免空格问题
    QString windeployqtPath = "windeployqt"; // 假设在PATH中
    QStringList arguments;

    // 解析命令参数
    QStringList parts = command.split(" ");
    QString exePath;
    QStringList windeployqtArgs;

    for (int i = 0; i < parts.size(); i++) {
        if (i == 0 && parts[i] == "windeployqt") {
            continue; // 跳过命令本身
        }
        if (parts[i].startsWith("\"") && !parts[i].endsWith("\"")) {
            // 处理带空格的路径
            QString fullPath = parts[i];
            while (++i < parts.size()) {
                fullPath += " " + parts[i];
                if (parts[i].endsWith("\"")) break;
            }
            if (exePath.isEmpty()) {
                exePath = fullPath;
            } else {
                windeployqtArgs << fullPath;
            }
        } else {
            if (exePath.isEmpty() && !parts[i].startsWith("--")) {
                exePath = parts[i];
            } else {
                windeployqtArgs << parts[i];
            }
        }
    }

    // 移除路径的引号（QProcess会处理）
    exePath = exePath.replace("\"", "");
    exePath = QDir::toNativeSeparators(exePath);

    // 检查文件是否存在
    if (!QFile::exists(exePath)) {
        QMessageBox::warning(this, translate("error"),
            QString(translate("file_not_exist")+":\n%1").arg(exePath));
        statusLabel->setText(translate("file_not_exist"));
        return;
    }

    // 构建参数列表
    arguments << exePath;
    arguments << windeployqtArgs;

    qDebug() << "Executing windeployqt with args:" << arguments;

    process.start(windeployqtPath, arguments);
    #else
    process.start("sh", QStringList() << "-c" << command);
    #endif

    if (process.waitForStarted()) {
        if (process.waitForFinished(60000)) { // 增加超时到60秒
            QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
            QString error = QString::fromLocal8Bit(process.readAllStandardError());

            QString result = "命令执行完成！\n\n";
            result += translate("exitcode") + QString::number(process.exitCode()) + "\n\n";

            if (!output.isEmpty()) {
                result += translate("output") + "\n" + output + "\n";
            }

            if (!error.isEmpty()) {
                result += translate("error")+"\n" + error + "\n";
            }

            // 保存日志到文件
            QString logPath = workingDir + "/deploy_log.txt";
            QFile logFile(logPath);
            if (logFile.open(QIODevice::WriteOnly)) {
                QTextStream stream(&logFile);
                stream << "Command: " << command << "\n\n";
                stream << result;
                logFile.close();
            }

            if (process.exitCode() == 0) {
                QMessageBox::information(this, translate("success"), result);
                statusLabel->setText(translate("success_deploy"));
            } else {
                QMessageBox::warning(this, translate("error"), result);
                statusLabel->setText(translate("warning_deploy"));
            }
        } else {
            process.kill();
            QMessageBox::critical(this, translate("error"), translate("timeout_deploy"));
            statusLabel->setText(translate("timeout_deploy"));
        }
    } else {
        QMessageBox::critical(this, translate("error"),
            QString(translate("unable_start_windeployqt")+"\n"+translate("error")+process.errorString()));
        statusLabel->setText(translate("startup_deploy"));
    }
}