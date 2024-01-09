import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
Rectangle
{
   anchors.fill: parent
   color: "#303030"
   id: batteryIndicator

   function setValue(value)
   {
      gauge.value = value;
   }
   CircularGauge
   {
      id: gauge
      width: parent.width
      height: parent.height
      y: parent.height / 4
      style: CircularGaugeStyle
      {
         property int warningValue: 5
         property int cautionValue: 33
         property
         var cautionColor: "#ff9800"
         property
         var warningColor: "#ff5722"
         property
         var defaultColor: "#64dd17"
         minimumValueAngle: -90.0
         maximumValueAngle: 90.0

         function degreesToRadians(degrees)
         {
            return degrees * (Math.PI / 180);
         }

         function valueToColor(value)
         {
            if (value > cautionValue)
            {
               return defaultColor;
            }
            else if (value > warningValue)
            {
               return cautionColor;
            }
            else
            {
               return warningColor;
            }
         }
         background: Canvas
         {
            onPaint:
            {
               var ctx = getContext("2d");
               ctx.reset();
               ctx.beginPath();
               ctx.strokeStyle = warningColor;
               ctx.lineWidth = outerRadius * 0.02;
               ctx.arc(outerRadius, outerRadius, outerRadius - ctx.lineWidth / 2, degreesToRadians(valueToAngle(0) - 90), degreesToRadians(valueToAngle(warningValue) - 90));
               ctx.stroke();
               ctx.beginPath();
               ctx.strokeStyle = cautionColor;
               ctx.lineWidth = outerRadius * 0.02;
               ctx.arc(outerRadius, outerRadius, outerRadius - ctx.lineWidth / 2, degreesToRadians(valueToAngle(warningValue) - 90), degreesToRadians(valueToAngle(cautionValue) - 90));
               ctx.stroke();
               ctx.beginPath();
               ctx.strokeStyle = defaultColor;
               ctx.lineWidth = outerRadius * 0.02;
               ctx.arc(outerRadius, outerRadius, outerRadius - ctx.lineWidth / 2, degreesToRadians(valueToAngle(cautionValue) - 90), degreesToRadians(valueToAngle(100) - 90));
               ctx.stroke();
            }
            Image
            {
               source: "qrc:/battery/battery.png"
               anchors.bottom: parent.verticalCenter
               anchors.bottomMargin: 5
               anchors.horizontalCenter: parent.horizontalCenter
               width: 64
               height: width
               fillMode: Image.PreserveAspectFit
            }
         }
         tickmarkLabel: Text
         {
            visible: styleData.value === 0 || styleData.value === 100
            font.pixelSize: Math.max(6, outerRadius * 0.1)
            text: styleData.value === 0 ? "E" : (styleData.value === 100 ? "F" : "")
            color: valueToColor(styleData.value)
            antialiasing: true
         }
         minorTickmark: Rectangle
         {
            visible: false
         }
         tickmark: Rectangle
         {
            visible: styleData.value < 80 || styleData.value % 10 == 0
            implicitWidth: outerRadius * 0.02
            antialiasing: true
            implicitHeight: outerRadius * 0.06
            color: valueToColor(styleData.value)
         }
      }
   }
}