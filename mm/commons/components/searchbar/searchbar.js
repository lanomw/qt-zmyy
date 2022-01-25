Component({
    options: {
        addGlobalClass: !0
    },
    properties: {
        cityname: {
            type: String,
            value: "定位中..."
        }
    },
    data: {},
    methods: {
        onsearch: function(t) {
            this.triggerEvent("input", t.detail);
        },
        jump: function(t) {
            wx.navigateTo({
                url: t.currentTarget.dataset.url
            });
        }
    }
});