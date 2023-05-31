#include "context.hpp"
#include "ui.hpp"

using namespace highway;

context::Context::Context(QObject *parent)
    : QObject(parent), _ui(new ui::UI(this)), _state(new state::State(this)){

                                              };
