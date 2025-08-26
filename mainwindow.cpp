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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), settings(new QSettings("MyCompany", "WindDeployGUI", this))
{
    setupUI();
    createConnections();
    loadSettings();
    setWindowTitle("Qt Deploy Tool GUI - windeployqt助手");
    resize(900, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 文件路径选择组
    QGroupBox *fileGroup = new QGroupBox("文件路径", this);
    QFormLayout *fileLayout = new QFormLayout(fileGroup);

    executableEdit = new QLineEdit(this);
    browseExecutableBtn = new QPushButton("浏览...", this);
    QHBoxLayout *exeLayout = new QHBoxLayout();
    exeLayout->addWidget(executableEdit);
    exeLayout->addWidget(browseExecutableBtn);

    qmlDirEdit = new QLineEdit(this);
    browseQmlDirBtn = new QPushButton("浏览...", this);
    QHBoxLayout *qmlLayout = new QHBoxLayout();
    qmlLayout->addWidget(qmlDirEdit);
    qmlLayout->addWidget(browseQmlDirBtn);

    fileLayout->addRow("可执行文件:", exeLayout);
    fileLayout->addRow("QML目录 (--qmldir):", qmlLayout);

    // 模块选择组
    QGroupBox *moduleGroup = new QGroupBox("模块选择", this);
    QVBoxLayout *moduleLayout = new QVBoxLayout(moduleGroup);

    selectModulesBtn = new QPushButton("选择要排除的模块", this);
    selectModulesBtn->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));

    selectedModulesDisplay = new QTextEdit(this);
    selectedModulesDisplay->setMaximumHeight(80);
    selectedModulesDisplay->setReadOnly(true);
    selectedModulesDisplay->setPlaceholderText("未选择任何模块 - 将部署所有依赖模块");

    moduleLayout->addWidget(selectModulesBtn);
    moduleLayout->addWidget(new QLabel("已选择的排除模块:"));
    moduleLayout->addWidget(selectedModulesDisplay);

    // 选项组
    QGroupBox *optionsGroup = new QGroupBox("部署选项", this);
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    noSystemD3dCompilerCheck = new QCheckBox("不部署D3D编译器 (--no-system-d3d-compiler)", this);
    noSystemDxcCompilerCheck = new QCheckBox("不部署DXC编译器 (--no-system-dxc-compiler)", this);
    noCompilerRuntimeCheck = new QCheckBox("不部署VC++运行时 (--no-compiler-runtime)", this);
    noTranslationsCheck = new QCheckBox("不部署翻译文件 (--no-translations)", this);
    noQuickImportCheck = new QCheckBox("不部署Qt Quick导入 (--no-quick-import)", this);
    noOpenGlSwCheck = new QCheckBox("不部署软件OpenGL库 (--no-opengl-sw)", this);
    noFfmpegCheck = new QCheckBox("不部署FFmpeg库 (--no-ffmpeg)", this);
    forceCheck = new QCheckBox("强制覆盖文件 (--force)", this);
    autoRunCheck = new QCheckBox("生成命令后自动运行", this);

    // 设置默认选中状态
    noSystemD3dCompilerCheck->setChecked(true);
    noSystemDxcCompilerCheck->setChecked(true);
    noCompilerRuntimeCheck->setChecked(true);
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
    generateBtn = new QPushButton("生成并运行命令", this);
    copyBtn = new QPushButton("复制命令", this);
    runBtn = new QPushButton("仅运行", this);

    generateBtn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    copyBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    runBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    buttonLayout->addWidget(generateBtn);
    buttonLayout->addWidget(copyBtn);
    buttonLayout->addWidget(runBtn);

    // 输出组
    QGroupBox *outputGroup = new QGroupBox("生成的命令", this);
    QVBoxLayout *outputLayout = new QVBoxLayout(outputGroup);
    commandOutput = new QTextEdit(this);
    commandOutput->setPlaceholderText("这里将显示生成的windeployqt命令...");
    commandOutput->setMaximumHeight(100);
    outputLayout->addWidget(commandOutput);

    // 状态栏
    statusLabel = new QLabel("就绪", this);
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
        selectedModulesDisplay->setPlainText("未选择任何组件 - 将部署所有依赖项");
    } else {
        // 创建友好的显示名称映射
        QMap<QString, QString> displayNames = {
            {"quick-import", "Qt Quick 1导入"},
            {"system-d3d-compiler", "系统D3D编译器"},
            {"system-dxc-compiler", "系统DXC编译器"},
            {"compiler-runtime", "编译器运行时"},
            {"translations", "翻译文件"},
            {"opengl-sw", "软件OpenGL库"},
            {"ffmpeg", "FFmpeg库"},
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
    QString file = QFileDialog::getOpenFileName(this, "选择可执行文件", "", "Executable Files (*.exe)");
    if (!file.isEmpty()) {
        executableEdit->setText(file);
    }
}

void MainWindow::browseQmlDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择QML目录");
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
    QString message = "目标目录包含以下文件和文件夹：\n\n";

    if (contents.isEmpty()) {
        message += "（空目录）";
    } else {
        for (const QString &item : contents) {
            message += "• " + item + "\n";
        }
    }

    message += "\n请选择操作：";
    QMessageBox::information(this, "目录内容", message);
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
    msgBox.setWindowTitle("目录清理");
    msgBox.setText("目标目录中包含其他文件，请选择操作：");
    msgBox.setIcon(QMessageBox::Question);

    QPushButton *clearButton = msgBox.addButton("清除所有文件", QMessageBox::DestructiveRole);
    QPushButton *moveButton = msgBox.addButton("移动到临时目录", QMessageBox::ActionRole);
    QPushButton *cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);

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
                    QMessageBox::warning(this, "警告",
                        QString("无法删除目录: %1").arg(fileInfo.fileName()));
                }
            } else {
                if (!QFile::remove(fileInfo.absoluteFilePath())) {
                    success = false;
                    QMessageBox::warning(this, "警告",
                        QString("无法删除文件: %1").arg(fileInfo.fileName()));
                }
            }
        }
    }

    if (success) {
        QMessageBox::information(this, "成功", "目录已清理完成");
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
        QMessageBox::critical(this, "错误", "无法创建临时目录");
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
                    QMessageBox::warning(this, "警告",
                        QString("无法移动目录: %1").arg(fileInfo.fileName()));
                }
            } else {
                if (!QFile::rename(fileInfo.absoluteFilePath(), destPath)) {
                    success = false;
                    QMessageBox::warning(this, "警告",
                        QString("无法移动文件: %1").arg(fileInfo.fileName()));
                }
            }
        }
    }

    if (success) {
        QMessageBox::information(this, "成功",
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
        QMessageBox::warning(this, "警告", "请先选择可执行文件！");
        return "";
    }

    QFileInfo exeInfo(exePath);
    if (!exeInfo.exists()) {
        QMessageBox::warning(this, "警告", "可执行文件不存在！");
        return "";
    }

    // 检查并清理目录
    QString exeDir = exeInfo.absolutePath();
    if (!checkAndCleanDirectory(exeDir)) {
        QMessageBox::information(this, "信息", "操作已取消");
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
    command += " --libdir lib";
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
    statusLabel->setText("命令已生成");

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
        statusLabel->setText("命令已复制到剪贴板");
    } else {
        QMessageBox::information(this, "提示", "没有可复制的命令内容");
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

    statusLabel->setText("正在执行命令...");
    QApplication::processEvents(); // 更新UI

    QProcess process;
    QFileInfo exeInfo(executableEdit->text());


    // QDir dir(executableEdit->text());
    // QString baseDir = exeInfo.absolutePath();
    // // 创建lib目录
    // if (!dir.exists("lib")) {
    //     if (!dir.mkdir("lib")) {
    //         QMessageBox::warning(this, "警告",
    //             QString("无法创建lib目录:\n%1/lib").arg(baseDir));
    //     }
    // } else {
    //     // 如果lib目录已存在，确保它是可写的
    //     QFileInfo libInfo(baseDir + "/lib");
    //     if (!libInfo.isWritable()) {
    //         QMessageBox::warning(this, "警告",
    //             QString("lib目录不可写:\n%1/lib").arg(baseDir));
    //     }
    // }
    //
    // // 创建plugins目录
    // if (!dir.exists("plugins")) {
    //     if (!dir.mkdir("plugins")) {
    //         QMessageBox::warning(this, "警告",
    //             QString("无法创建plugins目录:\n%1/plugins").arg(baseDir));
    //     }
    // }

    // 确保路径使用正确的分隔符
    QString workingDir = exeInfo.absolutePath();
    workingDir = QDir::toNativeSeparators(workingDir); // 转换为本地分隔符
    process.setWorkingDirectory(workingDir);

    if (!createFolder(workingDir+"/lib"))QMessageBox::warning(this, "警告",
            QString("无法创建lib目录:\n%1/lib").arg(workingDir));
    if (!createFolder(workingDir+"/plugins"))QMessageBox::warning(this, "警告",
            QString("无法创建plugins目录:\n%1/plugins").arg(workingDir));

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
        QMessageBox::warning(this, "错误",
            QString("可执行文件不存在:\n%1").arg(exePath));
        statusLabel->setText("文件不存在");
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
            result += "退出代码: " + QString::number(process.exitCode()) + "\n\n";

            if (!output.isEmpty()) {
                result += "输出:\n" + output + "\n";
            }

            if (!error.isEmpty()) {
                result += "错误:\n" + error + "\n";
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
                QMessageBox::information(this, "成功", result);
                statusLabel->setText("部署成功完成");
            } else {
                QMessageBox::warning(this, "警告", result);
                statusLabel->setText("部署完成但有警告");
            }
        } else {
            process.kill();
            QMessageBox::critical(this, "错误", "命令执行超时");
            statusLabel->setText("执行超时");
        }
    } else {
        QMessageBox::critical(this, "错误",
            QString("无法启动windeployqt进程\n错误: %1").arg(process.errorString()));
        statusLabel->setText("启动失败");
    }
}