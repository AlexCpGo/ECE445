function get_result() {
	$.ajax({
		url:'http:localhost:5000/api/result',
		type:"GET",
		dataType:'JSON',
		success:function (data) {
			console.log(data)
			$.each(data,function (k,v) {     // 循环遍历    "1":{"name":"小黑","count":1}  键为前者，值为后者
				var li = document.createElement('li');   // 创建li标签
				li.setAttribute('gps',k);   // 设置当前的属性名与属性值
				li.innerText = v.name + "(" + v.count + ')' ;   // 生成新的DOM
			})

		}
	})
}
setInterval(get_result,3000);   // 3秒

//var li = {1:{'gps':somevalue},
//			2:{"gps":somevalue},
//			};


var road_data={
		Process:{smooth:27,rough:14},
		Sensor:{ on:2,off:0},

		fHeader:[
	        {"categories":"Road Name"},
	        {"categories":"Seg Idx"},
	        {"categories":"Alarm"},
			{"categories":"Obstacle"},
			{"categories":"Location"},

    	],
		f:[
			{"road":"Road A","Idx": "No.2", "Alarm": 1,"Obstacle":0, "Location": "(30.521425°N, 120.722831°E)"},
	        {"road":"Road A","Idx": "No.25","Alarm": 1,"Obstacle":0,"Location": "(30.521534°N, 120.722924°E)"},
	        {"road":"Road A","Idx": "No.32","Alarm": 1,"Obstacle":0,"Location": "(30.521688°N, 120.722993°E)"},
			{"road":"Road B","Idx": "No.7","Alarm": 0,"Obstacle":1,"Location": "(30.522266°N, 120.723701°E)"},
	        {"road":"Road B","Idx": "No.19","Alarm": 0,"Obstacle":0,"Location": "(30.522343°N, 120.723562°E)"},
	        {"road":"Road B","Idx": "No.24","Alarm": 0,"Obstacle":1,"Location": "(30.522586°N, 120.723426°E)"},
		//li[4].gps
	 	],
		 //Remained work: Transfer the processed data into the jason here
	};

var vm = new Vue({
	el: '#content',
	data: road_data,
	methods: {
		details: function() {
			
		}
	}
})