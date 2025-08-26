#include "mainwindow.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>

ModuleSelectionDialog::ModuleSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("选择要排除的组件");
    setModal(true);
    resize(600, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);

    moduleList = new QListWidget(this);
    moduleList->setSelectionMode(QListWidget::MultiSelection);


    // 添加Qt库排除选项
    // 在 modelselectiondialog.cpp 中更新 Qt 库选项
    QMap<QString, QString> qtLibraryOptions = {
        // 3D 相关库
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
        {"3dquickscene3d", "3D Quick场景3D"},
        {"3drender", "3D渲染模块"},

    // ActiveQt 相关
        {"activeqt", "ActiveQt模块"},
        {"axbaseInternal", "ActiveX基础内部模块"},
        {"axcontainer", "ActiveX容器模块"},
        {"axserver", "ActiveX服务器模块"},

    // 图表和可视化
        {"bluetooth", "蓝牙模块"},
        {"bodymovinInternal", "Bodymovin内部模块"},
        {"charts", "图表模块"},
        {"chartsqml", "QML图表模块"},
        {"datavisualization", "数据可视化"},
        {"datavisualizationqml", "QML数据可视化"},

    // 核心模块
        {"concurrent", "并发模块"},
        {"core", "核心模块"},
        {"core5compat", "Core5兼容模块"},
        {"dbus", "DBus模块"},
        {"designer", "设计器模块"},
        {"designercomponentsInternal", "设计器组件内部模块"},
        {"devicediscoverysupportInternal", "设备发现支持内部模块"},
        {"entrypointInternal", "入口点内部模块"},

    // 示例和内部模块
        {"exampleiconsInternal", "示例图标内部模块"},
        {"examplesassetdownloaderInternal", "示例资源下载器内部模块"},
        {"fbsupportInternal", "帧缓冲支持内部模块"},
        {"freetypeInternal", "FreeType内部模块"},

    // 图形和渲染
        {"graphs", "图形模块"},
        {"graphswidgets", "图形控件模块"},
        {"grpc", "gRPC模块"},
        {"grpcquick", "Quick gRPC模块"},
        {"gui", "GUI模块"},
        {"harfbuzzInternal", "HarfBuzz内部模块"},
        {"qthelp", "Qt帮助模块"},
        {"httpserver", "HTTP服务器模块"},
        {"jpegInternal", "JPEG内部模块"},

    // JSON 和网络
        {"jsonrpcInternal", "JSON-RPC内部模块"},
        {"labsanimation", "实验室动画模块"},
        {"labsfolderlistmodel", "实验室文件夹列表模型"},
        {"labsplatform", "实验室平台模块"},
        {"labsqmlmodels", "实验室QML模型"},
        {"labssettings", "实验室设置模块"},
        {"labssharedimage", "实验室共享图像"},
        {"labswavefrontmesh", "实验室Wavefront网格"},
        {"languageserverInternal", "语言服务器内部模块"},
        {"linguist", "语言学家模块"},
        {"location", "定位模块"},

    // 多媒体
        {"multimedia", "多媒体模块"},
        {"multimediaquickInternal", "Quick多媒体内部模块"},
        {"multimediatestlibInternal", "多媒体测试库内部模块"},
        {"multimediawidgets", "多媒体控件模块"},

    // 网络和通信
        {"network", "网络模块"},
        {"networkauth", "网络认证模块"},
        {"nfc", "NFC模块"},
        {"opengl", "OpenGL模块"},
        {"openglwidgets", "OpenGL控件模块"},
        {"packetprotocolInternal", "数据包协议内部模块"},
        {"pngInternal", "PNG内部模块"},
        {"positioning", "定位模块"},
        {"positioningquick", "Quick定位模块"},
        {"printsupport", "打印支持模块"},

    // Protocol Buffers
        {"protobuf", "Protocol Buffers模块"},
        {"protobufqtcoretypes", "Protocol Buffers Qt核心类型"},
        {"protobufqtguitypes", "Protocol Buffers Qt GUI类型"},
        {"protobufquick", "Quick Protocol Buffers模块"},
        {"protobufwellknowntypes", "Protocol Buffers已知类型"},

    // QML 相关
        {"qdocInternal", "QDoc内部模块"},
        {"qml", "QML模块"},
        {"qmlassetdownloader", "QML资源下载器"},
        {"qmlcompiler", "QML编译器"},
        {"qmlcore", "QML核心模块"},
        {"qmldebugInternal", "QML调试内部模块"},
        {"qmldomInternal", "QML DOM内部模块"},
        {"qmlintegration", "QML集成模块"},
        {"qmllocalstorage", "QML本地存储"},
        {"qmllsInternal", "QML语言服务器内部模块"},
        {"qmlmeta", "QML元数据模块"},
        {"qmlmodels", "QML模型模块"},
        {"qmlnetwork", "QML网络模块"},
        {"qmltoolingsettingsInternal", "QML工具设置内部模块"},
        {"qmltyperegistrarInternal", "QML类型注册器内部模块"},
        {"qmlworkerscript", "QML工作脚本"},
        {"qmlxmllistmodel", "QML XML列表模型"},

    // Quick 相关
        {"quick", "Quick模块"},
        {"quick3d", "Quick3D模块"},
        {"quick3dassetimport", "Quick3D资源导入"},
        {"quick3dassetutils", "Quick3D资源工具"},
        {"quick3deffects", "Quick3D效果"},
        {"quick3dglslparserInternal", "Quick3D GLSL解析器内部模块"},
        {"quick3dhelpers", "Quick3D助手"},
        {"quick3dhelpersimpl", "Quick3D助手实现"},
        {"quick3diblbaker", "Quick3D IBL烘焙器"},
        {"quick3dparticleeffects", "Quick3D粒子效果"},
        {"quick3dparticles", "Quick3D粒子"},
        {"quick3dphysics", "Quick3D物理"},
        {"quick3dphysicshelpers", "Quick3D物理助手"},
        {"quick3druntimerender", "Quick3D运行时渲染"},
        {"quick3dspatialaudioInternal", "Quick3D空间音频内部模块"},
        {"quick3dutils", "Quick3D工具"},
        {"quick3dxr", "Quick3D XR模块"},
        {"quickcontrols2", "Quick Controls 2"},
        {"quickcontrols2basic", "Quick Controls 2基础"},
        {"quickcontrols2basicstyleimpl", "Quick Controls 2基础样式实现"},
        {"quickcontrols2fluentwinui3styleimpl", "Quick Controls 2 Fluent WinUI3样式实现"},
        {"quickcontrols2fusion", "Quick Controls 2 Fusion"},
        {"quickcontrols2fusionstyleimpl", "Quick Controls 2 Fusion样式实现"},
        {"quickcontrols2imagine", "Quick Controls 2 Imagine"},
        {"quickcontrols2imaginestyleimpl", "Quick Controls 2 Imagine样式实现"},
        {"quickcontrols2impl", "Quick Controls 2实现"},
        {"quickcontrols2material", "Quick Controls 2 Material"},
        {"quickcontrols2materialstyleimpl", "Quick Controls 2 Material样式实现"},
        {"quickcontrols2universal", "Quick Controls 2 Universal"},
        {"quickcontrols2universalstyleimpl", "Quick Controls 2 Universal样式实现"},
        {"quickcontrols2windowsstyleimpl", "Quick Controls 2 Windows样式实现"},
        {"quickcontrolstestutilsInternal", "Quick控件测试工具内部模块"},
        {"quickdialogs2", "Quick Dialogs 2"},
        {"quickdialogs2quickimpl", "Quick Dialogs 2 Quick实现"},
        {"quickdialogs2utils", "Quick Dialogs 2工具"},
        {"quickeffectsInternal", "Quick效果内部模块"},
        {"quicklayouts", "Quick布局"},
        {"quickparticlesInternal", "Quick粒子内部模块"},
        {"quickshapesInternal", "Quick形状内部模块"},
        {"quicktemplates2", "Quick Templates 2"},
        {"quicktest", "Quick测试"},
        {"quicktestutilsInternal", "Quick测试工具内部模块"},
        {"quicktimeline", "Quick时间线"},
        {"quicktimelineblendtrees", "Quick时间线混合树"},
        {"quickvectorimage", "Quick矢量图像"},
        {"quickvectorimagegeneratorInternal", "Quick矢量图像生成器内部模块"},
        {"quickwidgets", "Quick控件"},

    // 其他模块
        {"remoteobjects", "远程对象"},
        {"remoteobjectsqml", "QML远程对象"},
        {"repparser", "Rep解析器"},
        {"scxml", "SCXML模块"},
        {"scxmlqml", "QML SCXML模块"},
        {"sensors", "传感器模块"},
        {"sensorsquick", "Quick传感器模块"},
        {"serialbus", "串行总线"},
        {"serialport", "串行端口"},
        {"shadertools", "着色器工具"},
        {"spatialaudio", "空间音频"},
        {"sql", "SQL模块"},
        {"statemachine", "状态机"},
        {"statemachineqml", "QML状态机"},
        {"svg", "SVG模块"},
        {"svgwidgets", "SVG控件模块"},
        {"test", "测试模块"},
        {"texttospeech", "文本转语音模块"},
        {"toolsInternal", "工具内部模块"},
        {"uiplugin", "UI插件"},
        {"uitools", "UI工具"},
        {"virtualkeyboard", "虚拟键盘"},
        {"virtualkeyboardsettings", "虚拟键盘设置"},
        {"webchannel", "Web通道模块"},
        {"webchannelquick", "Quick Web通道模块"},
        {"websockets", "WebSocket模块"},
        {"webview", "WebView模块"},
        {"webviewquick", "Quick WebView模块"},
        {"widgets", "Widgets模块"},
        {"xml", "XML模块"},
        {"zlibInternal", "Zlib内部模块"}
    };

    // 添加插件类型排除选项（使用 skip-plugin-types）
    QMap<QString, QString> pluginTypeOptions = {
        {"platforms", "平台插件"},
        {"printsupport", "打印支持插件"},
        {"sensors", "传感器插件"},
        {"styles", "样式插件"},
        {"texttospeech", "文本转语音插件"},
        {"webview", "WebView插件"}
    };

    QMap<QString, QString> specificPluginOptions = {
        {"qcertonlybackend", "证书后端"},
        {"qopensslbackend", "OpenSSL后端"},
        {"qschannelbackend", "Schannel后端"},
        {"qmodernwindowsstyle", "现代Windows样式"},
        {"qsqlite", "SQLite驱动"},
        {"qsqlmimer", "Mimer SQL驱动"},
        {"qsqlodbc", "ODBC驱动"},
        {"qsqlpsql", "PostgreSQL驱动"},
        {"assimpsceneimport", "Assimp场景导入"},
        {"gltfsceneexport", "GLTF场景导出"},
        {"gltfsceneimport", "GLTF场景导入"},
        {"qscxmlecmascriptdatamodel", "SCXML ECMAScript数据模型"},
        {"scene2d", "2D场景"},
        {"qmldbg_debugger", "QML调试器"},
        {"qmldbg_inspector", "QML检查器"},
        {"qmldbg_local", "本地QML调试"},
        {"qmldbg_messages", "QML消息调试"},
        {"qmldbg_native", "原生调试"},
        {"qmldbg_nativedebugger", "原生调试器"},
        {"qmldbg_preview", "QML预览"},
        {"qmldbg_profiler", "QML性能分析器"},
        {"qmldbg_quick3dprofiler", "Quick3D性能分析器"},
        {"qmldbg_quickprofiler", "Quick性能分析器"},
        {"qmldbg_server", "QML调试服务器"},
        {"qmldbg_tcp", "TCP QML调试"},
        {"quicklintplugin", "Quick Lint插件"},
        {"openglrenderer", "OpenGL渲染器"},
        {"rhirenderer", "RHI渲染器"},
        {"qaxwidget", "ActiveX控件"},
        {"qquickwidget", "Quick控件"},
        {"qmllsquickplugin", "QML语言服务器插件"},
        {"qtuiotouchplugin", "UI触摸插件"},
        {"qtsensors_generic", "通用传感器"},
        {"qgif", "GIF图像格式"},
        {"qicns", "ICNS图像格式"},
        {"qico", "ICO图像格式"},
        {"qjpeg", "JPEG图像格式"},
        {"qsvg", "SVG图像格式"},
        {"qtga", "TGA图像格式"},
        {"qtiff", "TIFF图像格式"},
        {"qwbmp", "WBMP图像格式"},
        {"qwebp", "WebP图像格式"},
        {"ffmpegmediaplugin", "FFmpeg媒体插件"},
        {"windowsmediaplugin", "Windows媒体插件"},
        {"defaultgeometryloader", "默认几何加载器"},
        {"gltfgeometryloader", "GLTF几何加载器"},
        {"qtpassthrucanbus", "Passthru CAN总线"},
        {"qtpeakcanbus", "PEAK CAN总线"},
        {"qtsysteccanbus", "SYSTEC CAN总线"},
        {"qttinycanbus", "Tiny CAN总线"},
        {"qtvectorcanbus", "Vector CAN总线"},
        {"qtvirtualcanbus", "虚拟CAN总线"},
        {"qtgeoservices_itemsoverlay", "ItemsOverlay地理服务"},
        {"qtgeoservices_osm", "OSM地理服务"},
        {"helpplugin", "帮助插件"},
        {"qtvirtualkeyboardplugin", "虚拟键盘插件"},
        {"qtexttospeech_mock", "模拟文本转语音"},
        {"qtexttospeech_sapi", "SAPI文本转语音"},
        {"qsvgicon", "SVG图标引擎"},
        {"qnetworklistmanager", "网络列表管理器"},
        {"qtposition_nmea", "NMEA定位"},
        {"qtposition_positionpoll", "轮询定位"},
        {"assimp", "Assimp插件"},
        {"qdirect2d", "Direct2D平台"},
        {"qminimal", "最小化平台"},
        {"qoffscreen", "离屏平台"},
        {"qwindows", "Windows平台"}
    };

    // 添加Qt库选项分组
    QListWidgetItem *qtLibraryHeader = new QListWidgetItem("Qt库选项 (使用 --no- 前缀)", moduleList);
    qtLibraryHeader->setFlags(qtLibraryHeader->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
    qtLibraryHeader->setBackground(Qt::lightGray);

    for (auto it = qtLibraryOptions.constBegin(); it != qtLibraryOptions.constEnd(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1 (--no-%2)").arg(it.value()).arg(it.key()), moduleList);
        item->setData(Qt::UserRole, it.key());
        item->setData(Qt::UserRole + 1, "library"); // 标记为库选项
        item->setCheckState(Qt::Unchecked);
    }

    // 添加插件类型选项分组
    QListWidgetItem *pluginHeader = new QListWidgetItem("插件类型选项 (使用 --skip-plugin-types)", moduleList);
    pluginHeader->setFlags(pluginHeader->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
    pluginHeader->setBackground(Qt::lightGray);

    for (auto it = pluginTypeOptions.constBegin(); it != pluginTypeOptions.constEnd(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1 (--skip-plugin-types %2)").arg(it.value()).arg(it.key()), moduleList);
        item->setData(Qt::UserRole, it.key());
        item->setData(Qt::UserRole + 1, "plugin-type"); // 标记为插件类型选项
        item->setCheckState(Qt::Unchecked);
    }

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    layout->addWidget(new QLabel("选择要排除的组件（不部署）："));
    layout->addWidget(moduleList);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QSet<QString> ModuleSelectionDialog::getSelectedModules() const
{
    QSet<QString> modules;
    for (int i = 0; i < moduleList->count(); ++i) {
        QListWidgetItem *item = moduleList->item(i);
        if (item->checkState() == Qt::Checked && item->data(Qt::UserRole).isValid()) {
            modules.insert(item->data(Qt::UserRole).toString());
        }
    }
    return modules;
}

QSet<QString> ModuleSelectionDialog::getSelectedPluginTypes() const
{
    QSet<QString> pluginTypes;
    for (int i = 0; i < moduleList->count(); ++i) {
        QListWidgetItem *item = moduleList->item(i);
        if (item->checkState() == Qt::Checked && item->data(Qt::UserRole + 1).toString() == "plugin-type") {
            pluginTypes.insert(item->data(Qt::UserRole).toString());
        }
    }
    return pluginTypes;
}