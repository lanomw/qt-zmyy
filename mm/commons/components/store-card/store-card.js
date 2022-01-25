Component({
    options: {
        addGlobalClass: !0
    },
    behaviors: [],
    properties: {
        pic: String,
        status: String,
        title: String,
        titleIcon: String,
        descst: String,
        descstIcon: String,
        descnd: String,
        descndIcon: String,
        tip: String,
        bt: Array,
        rangeIndex: Number,
        rangeTotal: Number,
        titleTag: Boolean
    },
    data: {
        iserror: !1
    },
    lifetimes: {
        attached: function() {},
        moved: function() {},
        detached: function() {}
    },
    attached: function() {},
    ready: function() {},
    pageLifetimes: {
        show: function() {}
    },
    methods: {
        onclick: function(t) {
            var e = t.currentTarget.dataset.key, n = t.currentTarget.dataset.id, r = t.currentTarget.dataset.label;
            this.triggerEvent("click", {
                key: e,
                id: n,
                label: r
            });
        },
        loaderror: function(t) {
            this.setData({
                iserror: !0
            });
        }
    }
});