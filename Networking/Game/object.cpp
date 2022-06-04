//#include "object.h"
//
//Object::Object(OBJECT_TYPE type, PLAYER_STATE state, Point coors) : _type(type), _state(state), _coors(coors) {
//}
//
//void Object::set(Point coors, PLAYER_STATE state) {
//    _coors=coors;
//    _state=state;
//}
//
//const Object& Object::get() const {
//    return *this;
//}
//
//const Point& Object::coordinates() const {
//    return _coors;
//}
//
//const PLAYER_STATE& Object::state() const {
//    return _state;
//}
//
//Point operator+(const Point& first, const Point& second) {
//    return Point{first.x + second.x, first.y + second.y};
//}
