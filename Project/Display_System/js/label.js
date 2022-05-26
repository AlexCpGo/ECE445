function get_result() {
    $.ajax({
        url:'http:localhost:5000/api/result',
        type:"GET",
        dataType:'JSON',
        success:function (data) {
            console.log(data)
            $.each(data,function (k,v) {     // 循环遍历    "1":{"name":"小黑","count":1}  键为前者，值为后者
                var li = document.createElement('li');   // 创建li标签
                li.setAttribute('GPS',k);   // 设置当前的属性名与属性值
                li.innerText = v.name + "(" + v.count + ')' ;   // 生成新的DOM
            })

        }
    })
}