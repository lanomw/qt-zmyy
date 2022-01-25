Page({
    data: {},
    onShow: function() {
        getApp().dot();
    },
    redirectTo: function(t) {
        var r = t.currentTarget.dataset.url;
        wx.redirectTo({
            url: r
        });
    }
});