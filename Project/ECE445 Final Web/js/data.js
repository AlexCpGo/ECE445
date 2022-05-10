var road_data={
		Process:{smooth:333,rough:6},
		Sensor:{ on:2,off:0},

		factoryHeader:[
	        {"categories":"Road Name"},
	        {"categories":"Seg Idx"},
	        {"categories":"Label"},
			{"categories":"Alarm"},
    	],
		factory:[
			{"road":"HaiZhou East","Idx": "No.1", "label": 0,"alarm": "No"},
	        {"road":"HaiZhou East","Idx": "No.2","label": 1,"alarm": "Yes"},
	        {"road":"HaiZhou East","Idx": "No.1","label": 0,"alarm": "No"},
	        {"road":"HaiZhou West","Idx": "No.2","label": 0,"alarm": "No"},
	        {"road":"HaiZhou West","Idx": "No.3","label": 0,"alarm": "No"},

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