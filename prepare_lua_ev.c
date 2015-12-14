/**
 * Create a table for ev.Idle that gives access to the constructor for
 * prepare objects.
 *
 * [-0, +1, ?]
 */
static int luaopen_ev_prepare(lua_State *L) {
    lua_pop(L, create_prepare_mt(L));

    lua_createtable(L, 0, 1);

    lua_pushcfunction(L, prepare_new);
    lua_setfield(L, -2, "new");

    return 1;
}

/**
 * Create the prepare metatable in the registry.
 *
 * [-0, +1, ?]
 */
static int create_prepare_mt(lua_State *L) {

    static luaL_Reg fns[] = {
        { "stop",          prepare_stop },
        { "start",         prepare_start },
        { NULL, NULL }
    };
    luaL_newmetatable(L, PREPARE_MT);
    add_watcher_mt(L);
    luaL_setfuncs(L, fns, 0);

    return 1;
}

/**
 * Create a new prepare object.  Arguments:
 *   1 - callback function.
 *
 * @see watcher_new()
 *
 * [+1, -0, ?]
 */
static int prepare_new(lua_State* L) {
    ev_prepare*  prepare;

    prepare = watcher_new(L, sizeof(ev_prepare), PREPARE_MT);
    ev_prepare_init(prepare, &prepare_cb);
    return 1;
}

/**
 * @see watcher_cb()
 *
 * [+0, -0, m]
 */
static void prepare_cb(struct ev_loop* loop, ev_prepare* prepare, int revents) {
    watcher_cb(loop, prepare, revents);
}

/**
 * Stops the prepare so it won't be called by the specified event loop.
 *
 * Usage:
 *     prepare:stop(loop)
 *
 * [+0, -0, e]
 */
static int prepare_stop(lua_State *L) {
    ev_prepare*        prepare   = check_prepare(L, 1);
    struct ev_loop* loop   = *check_loop_and_init(L, 2);

    loop_stop_watcher(L, 2, 1);
    ev_prepare_stop(loop, prepare);

    return 0;
}

/**
 * Starts the prepare so it won't be called by the specified event loop.
 *
 * Usage:
 *     prepare:start(loop [, is_daemon])
 *
 * [+0, -0, e]
 */
static int prepare_start(lua_State *L) {
    ev_prepare*        prepare   = check_prepare(L, 1);
    struct ev_loop* loop   = *check_loop_and_init(L, 2);
    int is_daemon          = lua_toboolean(L, 3);
    ev_prepare_start(loop, prepare);
    loop_start_watcher(L, 2, 1, is_daemon);

    return 0;
}
