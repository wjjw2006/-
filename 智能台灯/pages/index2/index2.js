// miniprogram/pages/index2/index2.js
const app = getApp()
Page({
  data: {
    page_light:null,    
    page_str:null
  },
  OneNet_Post:function(){
    var that=this
    let deviceid = "657298466"
    let apikey = "lAm78PucdgHsv8nuGWcJIke2Iqg="
    let data={
      "datastreams": [
        {"id": "status","datapoints":[{"value": app.globalData.status}]},
        {"id": "light","datapoints":[{"value": that.data.page_light}]},
        {"id": "pattern","datapoints":[{"value": 0}]},
      ]
    }
    wx.request({
      url: "https://api.heclouds.com/devices/" + deviceid + "/datapoints",
      method:'POST',
      header:{
        "content-type": 'application/json',
        "api-key": apikey
      },
      data:JSON.stringify(data),
      success(res){
        console.log("更新数据成功",res)
      },
      fail: function(res){
        wx.showToast({ title: '系统错误' })
      },
      complete:function(res){
        wx.hideLoading()
      }
    })
  },
  switch_light: function(){
    app.globalData.status=1-app.globalData.status
    console.log(app.globalData.status)
    if(app.globalData.status ==1){
      app.globalData.str='开'
    }
    else{
      app.globalData.str='关'
    }
    this.setData({
      page_str: app.globalData.str
    })
    this.OneNet_Post();
  },
  backToFirst: function(options){
    wx.redirectTo({
      url: '../index/index',
    })
  },
  onLoad: function (options) {
    this.setData({
      page_str: app.globalData.str,
      page_light: app.globalData.light
    })
    this.OneNet_Post();
  }
  
})