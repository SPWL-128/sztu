
import cv2
import numpy as np
def preprocess_and_detect_qrcode(image):
    """
    任务一：图片预处理与二维码定位
    - 灰度化和高斯滤波
    - 使用QRCodeDetector检测和解码二维码
    返回：二维码信息、二维码边界框、处理后的图像
    """
    # 灰度化
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # 高斯滤波平滑图像
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    # 初始化二维码检测器
    detector = cv2.QRCodeDetector()
    # 检测和解码二维码
    qr_data, bbox, _ = detector.detectAndDecode(image)
    
    return qr_data, bbox, blurred
def detect_and_extract_face(image):
    """
    任务二：人脸检测与证件照提取
    - 使用Haar Cascade检测人脸
    - 裁剪并保存人脸区域
    返回：人脸边界框列表、裁剪的人脸图像
    """
    # 加载Haar级联分类器
    face_cascade = cv2.CascadeClassifier(
        cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
    )
    # 转换为灰度图（Haar检测需要灰度图）
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # 检测人脸
    faces = face_cascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30)
    )
    # 裁剪人脸区域（取第一个检测到的人脸）
    face_crop = None
    if len(faces) > 0:
        x, y, w, h = faces[0]
        face_crop = image[y:y+h, x:x+w].copy()
        # 保存裁剪的人脸图片
        cv2.imwrite('homework_final/face_crop.jpg', face_crop)
    
    return faces, face_crop


def locate_text_region(image):
    """
    任务三：基于颜色分割的文字区域定位
    - 二值化分割深色文字
    - 查找轮廓定位文字区域
    返回：文字区域边界框
    """
    # 转换为灰度图
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # 二值化：像素值<100的为文字（深色），设为白色；其他为黑色
    _, binary = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY_INV)
    # 形态学操作：膨胀连接相邻文字
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (15, 5))
    dilated = cv2.dilate(binary, kernel, iterations=2)
    # 查找轮廓
    contours, _ = cv2.findContours(dilated, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # 找到最大的连通区域（假设为主要文字区域）
    text_bbox = None
    if contours:
        # 按面积排序，取最大的轮廓
        largest_contour = max(contours, key=cv2.contourArea)
        text_bbox = cv2.boundingRect(largest_contour)
    
    return text_bbox, binary


def draw_detection_results(image, qr_bbox, faces, text_bbox):
    """
    在图像上绘制所有检测结果
    - 绿色框：二维码
    - 红色框：人脸
    - 蓝色框：文字区域
    """
    result_img = image.copy()
    
    # 绘制二维码边界框（绿色）
    if qr_bbox is not None:
        bbox = qr_bbox.astype(int)
        cv2.polylines(result_img, [bbox], True, (0, 255, 0), 3)
    # 绘制人脸边界框（红色）
    for (x, y, w, h) in faces:
        cv2.rectangle(result_img, (x, y), (x + w, y + h), (0, 0, 255), 3)
    # 绘制文字区域边界框（蓝色）
    if text_bbox is not None:
        x, y, w, h = text_bbox
        cv2.rectangle(result_img, (x, y), (x + w, y + h), (255, 0, 0), 3)
    
    return result_img


def generate_report(qr_data, faces):
    """
    任务四：生成验证报告
    在控制台打印验证结果
    """
    print("\n" + "=" * 35)
    print("=== 访客证件卡信息验证报告 ===")
    print("=" * 35)
    # 二维码信息
    if qr_data:
        print(f"1. 二维码信息：{qr_data}")
    else:
        print("1. 二维码信息：未检测到二维码")
    # 人脸检测结果
    face_count = len(faces)
    if face_count > 0:
        print(f"2. 人脸检测：成功 (检测到{face_count}张人脸)")
    else:
        print("2. 人脸检测：失败 (未检测到人脸)")
    # 文字区域
    print("3. 文字区域：已定位")
    
    print("=" * 35 + "\n")


def display_results(original, annotated, face_crop):
    """
    任务四：综合显示结果
    并排显示三张图：原始图、标注图、人脸裁剪图
    """
    # 获取原图尺寸
    h, w = original.shape[:2]
    
    # 调整人脸图片大小以便并排显示
    if face_crop is not None:
        # 将人脸图片调整为与原图等高
        face_h, face_w = face_crop.shape[:2]
        scale = h / face_h
        new_w = int(face_w * scale)
        face_resized = cv2.resize(face_crop, (new_w, h))
    else:
        # 如果没有检测到人脸，创建一个空白图像
        face_resized = np.zeros((h, w // 3, 3), dtype=np.uint8)
        cv2.putText(face_resized, "No Face", (10, h // 2),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
    
    # 添加标签
    original_labeled = original.copy()
    cv2.putText(original_labeled, "Original", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
    
    annotated_labeled = annotated.copy()
    cv2.putText(annotated_labeled, "Detection Result", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
    
    face_labeled = face_resized.copy()
    cv2.putText(face_labeled, "Face Crop", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
    
    # 水平拼接三张图
    result = np.hstack([original_labeled, annotated_labeled, face_labeled])
    
    return result


def main():
    """
    主函数：执行完整的访客证件卡处理流程
    """
    # 读取访客证件卡图片
    image_path =r"D:\desktop\3.png"
    image = cv2.imread(image_path)
    
    if image is None:
        print(f"错误：无法读取图片 {image_path}")
        print("请确保访客证件卡图片存在于 homework_final 文件夹中")
        return
    
    print("正在处理访客证件卡...")
    
    # 保存原始图像副本
    original = image.copy()
    
    # 任务一：预处理与二维码检测
    print("任务一：检测二维码...")
    qr_data, qr_bbox, _ = preprocess_and_detect_qrcode(image)
    if qr_data:
        print(f"  二维码内容：{qr_data}")
    else:
        print("  未检测到二维码")
    
    # 任务二：人脸检测与提取
    print("任务二：检测人脸...")
    faces, face_crop = detect_and_extract_face(image)
    print(f"  检测到 {len(faces)} 张人脸")
    if face_crop is not None:
        print("  人脸已保存")
    
    # 任务三：文字区域定位
    print("任务三：定位文字区域...")
    text_bbox, binary = locate_text_region(image)
    if text_bbox:
        print(f"  文字区域：x={text_bbox[0]}, y={text_bbox[1]}, w={text_bbox[2]}, h={text_bbox[3]}")
    
    # 绘制检测结果
    annotated = draw_detection_results(image, qr_bbox, faces, text_bbox)
    
    # 任务四：生成验证报告
    generate_report(qr_data, faces)
    
    # 综合显示结果
    final_display = display_results(original, annotated, face_crop)
    
    # 显示结果
    cv2.imshow('Visitor Card Verification System', final_display)
    
    # 保存结果图片
    cv2.imwrite('D:/desktop/detection_result.jpg', annotated)
    cv2.imwrite('D:/desktop/final_display.jpg', final_display)
    print("结果已保存：")
    print("detection_result.jpg (标注图)")
    print("final_display.jpg (综合展示图)")
    print("face_crop.jpg (人脸裁剪图)")
    
    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()

