from ultralytics import YOLO
# Load a model
model = YOLO("yolov8n.pt")  # load an official model
model = YOLO("/home/ddxy/下载/ultralytics-main/best.pt")  # load a custom trained model
# Export the model
model.export(format="onnx")
