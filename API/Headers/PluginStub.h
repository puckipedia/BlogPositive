#if STANDALONE
#define MODULES extern "C" void loadModules(PluginList* list)
#define MODULE(__plugin__) list->AddItem(new __plugin__ ());
#endif
