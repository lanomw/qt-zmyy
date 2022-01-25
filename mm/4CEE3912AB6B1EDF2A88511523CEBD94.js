var e, n = require("467620F5AB6B1EDF201048F2888EBD94.js"), i = require("28BD41D3AB6B1EDF4EDB29D4C73EBD94.js");

null == e && (e = new i({
    key: n.qqmapkey
}));

var o = function(n, i) {
    return new Promise(function(o, t) {
        e.reverseGeocoder({
            location: {
                latitude: n,
                longitude: i
            },
            success: function(e) {
                e.latitude = n, e.longitude = i, o(e);
            },
            fail: function(e) {
                t(e);
            }
        });
    });
};

module.exports = {
    ReverseGeocoder: function(e, n) {
        return new Promise(function(i, t) {
            o(e, n).then(function(e) {
                i(e);
            }, function(e) {
                t(e);
            });
        });
    }
};