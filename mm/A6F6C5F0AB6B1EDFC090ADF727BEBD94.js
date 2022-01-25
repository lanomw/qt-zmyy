var t = wx.getRealtimeLogManager ? wx.getRealtimeLogManager() : null, e = getApp();

e && e.globalData && t.setFilterMsg(e.globalData.SDKVersion), module.exports = {
    info: function() {
        t && t.info.apply(t, arguments);
    },
    warn: function() {
        t && t.warn.apply(t, arguments);
    },
    error: function() {
        t && t.error.apply(t, arguments);
    },
    setFilterMsg: function(e) {
        t && t.setFilterMsg && "string" == typeof e && t.setFilterMsg(e);
    },
    addFilterMsg: function(e) {
        t && t.addFilterMsg && "string" == typeof e && t.addFilterMsg(e);
    }
};