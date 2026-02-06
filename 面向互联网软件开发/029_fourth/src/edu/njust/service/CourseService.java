package edu.njust.service;

import java.util.Map;

import edu.njust.dao.CourseDAO;
import edu.njust.entity.Course;
import edu.njust.entity.CourseResult;

public class CourseService {
	private CourseDAO courseDAO = new CourseDAO();
    public CourseResult addCourse(Course course) {
    	CourseResult result=new CourseResult();
    	if (course != null && course.getcName() != null && course.getcNum()!=null 
    			&& course.getcType() != null && !course.getcName().trim().isEmpty()
    			&& !course.getcNum().trim().isEmpty() && !course.getcType().trim().isEmpty()) {
            result.setSuccess(true);
            result.setMessage("添加成功");
            courseDAO.insertCourse(course);
            System.out.println(course.getcName()+" "+course.getcNum()+" "+course.getcType());
        } else {
            result.setSuccess(false);
            if(course==null) {
            	result.setErrorCode("1");
            	result.setMessage("添加失败，不能为空");
            }
            else if(course.getcName()==null||course.getcName().trim().isEmpty()) {
            	result.setErrorCode("2");
            	result.setMessage("添加失败，课程名不能为空");
            }
            else if(course.getcNum()==null||course.getcNum().trim().isEmpty()) {
            	result.setErrorCode("3");
            	result.setMessage("添加失败，选课人数不能为空");
            }
            else if(course.getcType()==null||course.getcType().trim().isEmpty()) {
            	result.setErrorCode("4");
            	result.setMessage("添加失败，课程性质不能为空");
            }
            else {
            	result.setErrorCode("5");
            	result.setMessage("添加失败");
            }
        }
        return result;
    }

    public void updateCourse(Course course) {
        courseDAO.updateCourse(course);
    }

    public CourseResult deleteCourse(String id) {
    	System.out.println(id+"删除id");
    	CourseResult result=new CourseResult();
    	CourseDAO dao=new CourseDAO();
    	Course course = dao.getCourseById(id);
    	System.out.println(course.getcType()+"删除课程类型");
        if (course != null && !course.getcType().equals("必修")) {
        	result.setSuccess(true);
        	result.setMessage("找到该课程，删除成功");
        	courseDAO.deleteCourse(id);
        } else if(course.getcType().equals("必修")){
        	result.setErrorCode("2");
        	result.setSuccess(false);
        	result.setMessage("必修课程,删除失败");
        }else {
            // 如果找不到该id的课程
        	result.setErrorCode("3");
        	result.setSuccess(false);
        	result.setMessage("找不到该课程,删除失败");
        }
        return result;
    }

    public Map<String, Course> getAllCourses() {
        return courseDAO.getAllCourses();
    }

    public Course getCourseById(String id) {
        return courseDAO.getCourseById(id);
    }
}
