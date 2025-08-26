#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QFileDialog>
#include <QSettings>
#include <QSet>
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QMap>

// 模块选择对话框
class ModuleSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModuleSelectionDialog(QWidget *parent = nullptr);
    QSet<QString> getSelectedModules() const;
    QSet<QString> getSelectedPluginTypes() const;

private:
    QListWidget *moduleList;
    QDialogButtonBox *buttonBox;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseExecutable();
    void browseQmlDir();
    void generateCommand();
    void copyCommand();
    void runCommand();
    void onExecutableChanged(const QString &path);
    void selectModules();

private:
    void setupUI();
    void createConnections();
    void loadSettings();
    void saveSettings();
    QString generateCommandString();
    bool checkAndCleanDirectory(const QString &dirPath);
    bool moveToTempDirectory(const QString &sourceDir, const QString &exePath);
    bool clearDirectory(const QString &dirPath, const QString &exePath);
    QSet<QString> getDirectoryContents(const QString &dirPath);
    void showDirectoryContents(const QSet<QString> &contents);
    void updateModulesDisplay();
    QSet<QString> getSelectedPluginTypes() const;
    bool createFolder(const QString &filePath);
    bool loadTranslations();
    QString translate(const QString &key);

    
    // 输入控件
    QLineEdit *executableEdit;
    QLineEdit *qmlDirEdit;

    // 选项控件
    QCheckBox *noSystemD3dCompilerCheck;
    QCheckBox *noCompilerRuntimeCheck;
    QCheckBox *noTranslationsCheck;
    QCheckBox *noVirtualKeyboardCheck;
    QCheckBox *forceCheck;
    QCheckBox *autoRunCheck;
    QCheckBox *noSystemDxcCompilerCheck;
    QCheckBox *noQuickImportCheck;
    QCheckBox *noOpenGlSwCheck;
    QCheckBox *noFfmpegCheck;


    // 按钮
    QPushButton *browseExecutableBtn;
    QPushButton *browseQmlDirBtn;
    QPushButton *selectModulesBtn;
    QPushButton *generateBtn;
    QPushButton *copyBtn;
    QPushButton *runBtn;

    // 输出
    QTextEdit *commandOutput;
    QTextEdit *selectedModulesDisplay;

    // 状态标签
    QLabel *statusLabel;

    // 设置
    QSettings *settings;

    // 选择的模块
    QSet<QString> selectedModules;

    // 选择的插件类型
    QSet<QString> selectedPluginTypes;

    QSet<QString> selectedSpecificPlugins;

    QSet<QString> selectedSpecificPluginTypes;

    //翻译
    QMap<QString, QString> translationMap;
    QString currentLanguage;
};

#endif // MAINWINDOW_H