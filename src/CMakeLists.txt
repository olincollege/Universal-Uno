# If you have a program that's just a .c file and it has a main method, define
# an executable. If it has both a .c and .h file, make it a library. Also, link
# libraries appropriately here. Targets should be added in dependency order - if
# executable bar needs library foo, then you should do
# add_library(foo foo.c foo.h)
# add_executable(bar bar.c)
# target_link_libraries(bar PRIVATE foo)
# or similar. The PRIVATE/INTERFACE/PUBLIC keyword will depend on whether the
# library is used only in function bodies (PRIVATE), only in function
# signatures/types (INTERFACE), or both (PUBLIC).

# add_library(utils utils.c utils.h)
# add_library(model model.c model.h)
add_library(view view.c view.h)

# add_library(util utils.c utils.h)

# add_library(client client.c client.h)
# target_link_libraries(client PRIVATE util)

# add_executable(run_client run_client.c)
# target_link_libraries(run_client PRIVATE client util view)

# add_library(server server.c server.h)
# target_link_libraries(server PRIVATE util)

# add_executable(run_server run_server.c)
# target_link_libraries(run_server PRIVATE server util model)
