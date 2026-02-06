package edu.njust.entity;

public class LoginResult {
	private boolean success; 
    private String message; 
    private String errorCode; 

    
    public LoginResult() {}
    public LoginResult(boolean success, String message, String errorCode) {
        this.setSuccess(success);
        this.setMessage(message);
        this.setErrorCode(errorCode);
    }

	public boolean isSuccess() {
		return success;
	}

	public void setSuccess(boolean success) {
		this.success = success;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public String getErrorCode() {
		return errorCode;
	}

	public void setErrorCode(String errorCode) {
		this.errorCode = errorCode;
	}
}
