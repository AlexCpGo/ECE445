$(function() {
	var dom = document.getElementById("container");
	var myChart = echarts.init(dom);
	var app = {};
	option = null;
	app.title = 'donut chart';
	
	option = {
	    tooltip: {
	        trigger: 'item',
	        formatter: "{a}  <br/> {b}: {c} ({d}%)"
	    },
	    legend: {
	        orient: 'horizontal',
	        x: 'left',
	        data:['Online','Offline'],
	        itemWidth: 8,
	        itemHeight: 8,
	        textStyle:{//Set the style of the legend
	            color:'#fff',
	            fontSize:12
	        }
	    },
	    color:['#37a2da','#ffd85c'],
	    series: [
	        {
	            name:'Sensor',
	            type:'pie',
	            radius: ['90%', '70%'],
	            avoidLabelOverlap: false,
	            label: {
	                normal: {
	                    show: false,
	                    position: 'center'
	                },
	                emphasis: {
	                    show: true,
	                    textStyle: {
	                        fontSize: '30',
	                        color:'#00fcff',
	                        fontWeight: 'bold'
	                    }
	                }
	            },
	            labelLine: {
	                normal: {
	                    show: false,
	                }
	            },
	            data:[
	                {value:vm.Sensor.on, name:'Online'},
	                {value:vm.Sensor.off, name:'Offline'}
	            ]
	        }
	    ]
	};

	if (option && typeof option === "object") {
	    myChart.setOption(option, true);
	}
});