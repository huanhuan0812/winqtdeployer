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

    moduleTree = new CheckBoxTree(this);
    layout->addWidget(moduleTree);
    moduleTree->setHeaderLabel("选择要排除的模块");


    // 添加Qt库排除选项
    // 在 modelselectiondialog.cpp 中更新 Qt 库选项
    QMap<QString, QString> qtLibraryOptions = {

    // 图表和可视化
        {"bodymovinInternal", "Bodymovin内部模块"},

    // 核心模块
        {"devicediscoverysupportInternal", "设备发现支持内部模块"},

    // 示例和内部模块
        {"exampleiconsInternal", "示例图标内部模块"},
        {"examplesassetdownloaderInternal", "示例资源下载器内部模块"},


    // JSON 和网络
        {"jsonrpcInternal", "JSON-RPC内部模块"},


    // QML 相关
        {"qdocInternal", "QDoc内部模块"},


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
        {"repparser", "Rep解析器"},
        {"scxml", "SCXML模块"},
        {"scxmlqml", "QML SCXML模块"},
        {"sensorsquick", "Quick传感器模块"},
        {"shadertools", "着色器工具"},
        {"spatialaudio", "空间音频"},
        {"sql", "SQL模块"},
        {"statemachine", "状态机"},
        {"statemachineqml", "QML状态机"},
        {"svg", "SVG模块"},
        {"svgwidgets", "SVG控件模块"},
        {"test", "测试模块"},
        {"texttospeech", "文本转语音模块"},
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
        {"xml", "XML模块"}
    };

    //fill librarytree 3d
    {
        QTreeWidgetItem *qt3d = moduleTree->addTopLevelItem("Qt 3d模块", "qt3d");
        moduleTree->addChildItem(qt3d,"3D 动画模块","--no-3danimation");
        moduleTree->addChildItem(qt3d,"3D 核心模块","--no-3dcore");
        moduleTree->addChildItem(qt3d,"3D额外模块" , "--no-3dextras");
        moduleTree->addChildItem(qt3d,"3D输入模块","--no-3dinput");
        moduleTree->addChildItem(qt3d,"3D逻辑模块","--no-3dlogic");
        QTreeWidgetItem *qt3dquick=moduleTree->addChildItem(qt3d,"3D Quick","3dquick");
        moduleTree->addChildItem(qt3dquick,"3D Quick模块","--no-3dquick");
        moduleTree->addChildItem(qt3dquick,"3D Quick动画","--no-3dquickanimation");
        moduleTree->addChildItem(qt3dquick,"3D Quick额外模块","--no-3dquickextras");
        moduleTree->addChildItem(qt3dquick,"3D Quick输入","--no-3dquickinput");
        moduleTree->addChildItem(qt3dquick,"3D Quick渲染","--no-3dquickrender");
        moduleTree->addChildItem(qt3dquick,"3D Quick场景2D","--no-3dquickscene2d");
        moduleTree->addChildItem(qt3dquick,"3D Quick场景3D","--no-3dquickscene3d");
        moduleTree->addChildItem(qt3d,"3D渲染模块","--no-3drender");
    }

    //fill library activeqt
    {
        QTreeWidgetItem *qtactive = moduleTree->addTopLevelItem("Qt Active", "qtactive");
        moduleTree->addChildItem(qtactive,"ActiveQt模块","--no-activeqt");
        moduleTree->addChildItem(qtactive,"ActiveX基础内部模块","--no-axbaseInternal");
        moduleTree->addChildItem(qtactive,"ActiveX容器模块","--no-axcontainer");
        moduleTree->addChildItem(qtactive,"ActiveX服务器模块","--no-axserver");
    }

    //library core
    {
        QTreeWidgetItem *qtcore = moduleTree->addTopLevelItem("Qt 核心", "qtcore");
        moduleTree->addChildItem(qtcore,"核心模块","--no-core");
        moduleTree->addChildItem(qtcore,"Core5兼容模块","--no-core5compat");
        moduleTree->addChildItem(qtcore,"入口点内部模块","--no-entrypointInternal");

        QTreeWidgetItem *qtmutiple=moduleTree->addChildItem(qtcore,"并发与多线程","qtmutiple");
        {
            moduleTree->addChildItem(qtmutiple,"并发模块","--no-concurrent");
            moduleTree->addChildItem(qtmutiple,"QML工作脚本","--no-qmlworkerscript");
        }

        QTreeWidgetItem *qtsys=moduleTree->addChildItem(qtcore,"系统间通信","qtipc");
        {
            moduleTree->addChildItem(qtsys,"DBus模块","--no-dbus");
            moduleTree->addChildItem(qtsys," 远程对象","--no-remoteobjects");
            moduleTree->addChildItem(qtsys," QML远程对象","--no-remoteobjectsqml");
        }

        QTreeWidgetItem *qtinternal=moduleTree->addChildItem(qtcore,"内部基础支持","qtinternal");
        {
            moduleTree->addChildItem(qtinternal,"工具内部模块","--no-toolsInternal");
            moduleTree->addChildItem(qtinternal," 数据包协议内部模块","--no-packetprotocolInternal");
            moduleTree->addChildItem(qtinternal," 帧缓冲支持内部模块","--no-fbsupportInternal");
        }
        moduleTree->addChildItem(qtcore,"Qt帮助模块","--no-qthelp");
    }

    //library gui
    QTreeWidgetItem *qtgui = moduleTree->addTopLevelItem("Qt GUI与多媒体", "qtgui");
    {
        moduleTree->addChildItem(qtgui,"GUI模块","--no-gui");

        QTreeWidgetItem *qtimage=moduleTree->addChildItem(qtgui,"图像格式支持","qtimage");
        {
            moduleTree->addChildItem(qtimage,"JPEG内部模块","--no-jpegInternal");
            moduleTree->addChildItem(qtimage,"PNG内部模块","--no-pngInternal");
            moduleTree->addChildItem(qtimage,"FreeType内部模块","--no-freetypeInternal");
            moduleTree->addChildItem(qtimage,"Zlib内部模块","--no-zlibInternal");
        }

        QTreeWidgetItem *qtgraphic=moduleTree->addChildItem(qtgui,"图形与渲染","qtgraphic");
        {
            moduleTree->addChildItem(qtgraphic,"OpenGL模块","--no-opengl");
            moduleTree->addChildItem(qtgraphic,"OpenGL控件模块","--no-openglwidgets");
            moduleTree->addChildItem(qtgraphic,"图形模块","--no-graphs");
            moduleTree->addChildItem(qtgraphic,"图形控件模块","--no-graphswidgets");
            moduleTree->addChildItem(qtgraphic,"着色器工具模块","--no-shadertools");
            moduleTree->addChildItem(qtgraphic,"HarfBuzz内部模块","--no-harfbuzzInternal");
        }

        QTreeWidgetItem *qttext=moduleTree->addChildItem(qtgui,"文本与字体","qttext");
        {
            moduleTree->addChildItem(qttext,"文本转语音模块","--no-texttospeech");
            moduleTree->addChildItem(qttext,"字体数据库内部模块","--no-fontdatabaseInternal");
        }
    }

    //library media
    QTreeWidgetItem *qtmedia=moduleTree->addTopLevelItem("多媒体支持","qtmedia");
    {
        moduleTree->addChildItem(qtmedia,"多媒体模块","--no-multimedia");
        moduleTree->addChildItem(qtmedia,"Quick多媒体内部模块","--no-multimediaquickInternal");
        moduleTree->addChildItem(qtmedia,"多媒体测试库内部模块","--no-multimediatestlibInternal");
        moduleTree->addChildItem(qtmedia,"多媒体控件模块","--no-multimediawidgets");
        moduleTree->addChildItem(qtmedia,"空间音频模块","--no-spatialaudio");
    }

    //library network
    QTreeWidgetItem *qtnetwork = moduleTree->addTopLevelItem("Qt 网络", "qtnetwork");
    {
        moduleTree->addChildItem(qtnetwork,"网络模块","--no-network");
        moduleTree->addChildItem(qtnetwork,"网络认证模块","--no-networkauth");
        moduleTree->addChildItem(qtnetwork,"位置模块","--no-location");
        moduleTree->addChildItem(qtnetwork,"HTTP服务器模块","--no-httpserver");
        moduleTree->addChildItem(qtnetwork,"gRPC模块","--no-grpc");
        moduleTree->addChildItem(qtnetwork,"Quick gRPC模块","--no-grpcquick");
    }

    //library lab
    QTreeWidgetItem *qtlab= moduleTree->addTopLevelItem("Qt 实验模块", "qtlab");
    {
        moduleTree->addChildItem(qtlab,"实验动画模块","--no-labsanimation");
        moduleTree->addChildItem(qtlab,"实验文件夹列表模型","--no-labsfolderlistmodel");
        moduleTree->addChildItem(qtlab,"实验平台模块","--no-labsplatform");
        moduleTree->addChildItem(qtlab,"实验QML模型","--no-labsqmlmodels");
        moduleTree->addChildItem(qtlab,"实验设置模块","--no-labssettings");
        moduleTree->addChildItem(qtlab,"实验共享图像","--no-labssharedimage");
        moduleTree->addChildItem(qtlab,"实验Wavefront网格","--no-labswavefrontmesh");
    }

    //library  Protocol Buffers
    QTreeWidgetItem *qtprotobuf= moduleTree->addTopLevelItem("Qt Protocol Buffers", "qtprotobuf");
    {
        moduleTree->addChildItem(qtprotobuf,"Protocol Buffers模块","--no-protobuf");
        moduleTree->addChildItem(qtprotobuf,"Protocol Buffers Qt核心类型","--no-protobufqtcoretypes");
        moduleTree->addChildItem(qtprotobuf,"Protocol Buffers Qt GUI类型","--no-protobufqtguitypes");
        moduleTree->addChildItem(qtprotobuf,"Quick Protocol Buffers模块","--no-protobufquick");
        moduleTree->addChildItem(qtprotobuf,"Protocol Buffers已知类型","--no-protobufwellknowntypes");
    }

    //library QML
    QTreeWidgetItem *qtqml= moduleTree->addTopLevelItem("Qt QML", "qtqml");
    {
        moduleTree->addChildItem(qtqml,"QML模块","--no-qml");
        moduleTree->addChildItem(qtqml,"QML核心模块","--no-qmlcore");
        moduleTree->addChildItem(qtqml,"QML网络模块","--no-qmlnetwork");
        moduleTree->addChildItem(qtqml,"QML模型模块","--no-qmlmodels");
        moduleTree->addChildItem(qtqml,"QML本地存储模块","--no-qmllocalstorage");
        moduleTree->addChildItem(qtqml,"QML集成模块","--no-qmlintegration");
        moduleTree->addChildItem(qtqml,"QML编译器模块","--no-qmlcompiler");
        moduleTree->addChildItem(qtqml,"QML调试内部模块","--no-qmldebugInternal");
        moduleTree->addChildItem(qtqml,"QML DOM内部模块","--no-qmldomInternal");
        moduleTree->addChildItem(qtqml,"QML元数据模块","--no-qmlmeta");
        moduleTree->addChildItem(qtqml,"QML语言服务器内部模块","--no-qmllsInternal");
        moduleTree->addChildItem(qtqml,"QML工具设置内部模块","--no-qmltoolingsettingsInternal");
        moduleTree->addChildItem(qtqml,"QML类型注册器内部模块","--no-qmltyperegistrarInternal");
        moduleTree->addChildItem(qtqml,"QML XML列表模型","--no-qmlxmllistmodel");
        moduleTree->addChildItem(qtqml,"QML资源下载器","--no-qmlassetdownloader");
    }

    //library land translate
    QTreeWidgetItem *qtlinguist= moduleTree->addTopLevelItem("Qt 语言翻译", "qtlinguist");
    {
        moduleTree->addChildItem(qtlinguist,"语言学家模块","--no-linguist");
        moduleTree->addChildItem(qtlinguist,"语言服务器内部模块","--no-linguistserverInternal");
    }

    //library hardware
    QTreeWidgetItem *qthardware= moduleTree->addTopLevelItem("Qt 硬件与传感器", "qthardware");
    {
        moduleTree->addChildItem(qthardware,"串行总线模块","--no-serialbus");
        moduleTree->addChildItem(qthardware,"串行端口模块","--no-serialport");
        moduleTree->addChildItem(qthardware,"传感器模块","--no-sensors");
        moduleTree->addChildItem(qthardware,"蓝牙模块","--no-bluetooth");
        moduleTree->addChildItem(qthardware,"NFC模块","--no-nfc");
        moduleTree->addChildItem(qthardware,"位置模块","--no-positioning");
        moduleTree->addChildItem(qthardware,"位置Quick模块","--no-positioningquick");
        moduleTree->addChildItem(qthardware,"打印支持模块","--no-printsupport");
    }

    //library design
    QTreeWidgetItem *qtdesign= moduleTree->addTopLevelItem("Qt 设计与帮助", "qtdesign");
    {
        moduleTree->addChildItem(qtdesign,"设计器模块","--no-designer");
        moduleTree->addChildItem(qtdesign,"设计器组件内部模块","--no-designercomponentsInternal");
    }

    //library chart
    QTreeWidgetItem *qtchart= moduleTree->addTopLevelItem("Qt 图表", "qtchart");
    {
        moduleTree->addChildItem(qtchart,"图表模块","--no-charts");
        moduleTree->addChildItem(qtchart,"QML图表模块","--no-chartsqml");
        moduleTree->addChildItem(qtchart,"数据可视化","--no-datavisualization");
        moduleTree->addChildItem(qtchart,"QML数据可视化","--no-datavisualizationqml");
    }

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