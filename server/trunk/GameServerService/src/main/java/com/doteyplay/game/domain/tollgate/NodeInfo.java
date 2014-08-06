package com.doteyplay.game.domain.tollgate;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/**
 * @className:RoleMap.java
 * @classDescription:��ɫ��ĳһ���ؿ��ڽ���,�������.
 * @author:Tom.Zheng
 * @createTime:2014��6��23�� ����2:37:14
 */
public class NodeInfo {

	private long roleId;
	/**
	 * �ؿ�Id
	 */
	private int tollgateId;

	/**
	 * �ڵ�Id
	 */
	private int nodeId;

	/**
	 * �Ƿ���.
	 */
	private byte isOpen;

	/**
	 * �Ƿ�ͨ��.
	 */
	private byte isPass;

	/**
	 * ����״̬.
	 */
	private byte starResult;

	/**
	 * ����ʱ��.
	 */

	private long beginTimestamp;

	/**
	 * ����ʱ��
	 */
	private long closeTimestamp;
	
	/**
	 * ����
	 */
	private int times;

	/**
	 * 
	 * @author:Tom.Zheng
	 * @createTime:2014��6��24�� ����2:12:14
	 * @param roleId
	 *            ��ɫId
	 * @param tollgateId
	 *            �ؿ�Id
	 * @param nodeId
	 *            ��ǰ�ڵ�Id
	 */
	public NodeInfo(long roleId, int tollgateId, int nodeId) {
		super();
		this.roleId = roleId;
		this.tollgateId = tollgateId;
		this.nodeId = nodeId;
	}

	public NodeInfo(long roleId, int tollgateId) {
		this(roleId, tollgateId, 0);
	}

	public long getRoleId() {
		return roleId;
	}

	public void setRoleId(long roleId) {
		this.roleId = roleId;
	}

	public int getTollgateId() {
		return tollgateId;
	}

	public void setTollgateId(int tollgateId) {
		this.tollgateId = tollgateId;
	}

	public byte getIsOpen() {
		return isOpen;
	}

	public void setIsOpen(byte isOpen) {
		this.isOpen = isOpen;
	}

	public byte getIsPass() {
		return isPass;
	}

	public void setIsPass(byte isPass) {
		this.isPass = isPass;
	}

	public byte getStarResult() {
		return starResult;
	}

	public void setStarResult(byte starResult) {
		this.starResult = starResult;
	}

	public long getBeginTimestamp() {
		return beginTimestamp;
	}

	public void setBeginTimestamp(long beginTimestamp) {
		this.beginTimestamp = beginTimestamp;
	}

	public long getCloseTimestamp() {
		return closeTimestamp;
	}

	public void setCloseTimestamp(long closeTimestamp) {
		this.closeTimestamp = closeTimestamp;
	}

	public int getNodeId() {
		return nodeId;
	}

	public void setNodeId(int nodeId) {
		this.nodeId = nodeId;
	}
	
	

	public int getTimes() {
		return times;
	}

	public void setTimes(int times) {
		this.times = times;
	}

	public void parseData(DataInputStream in, int ver) {
		// TODO Auto-generated method stub
		try {
			nodeId = in.readInt();
			isOpen = in.readByte();
			isPass = in.readByte();
			starResult = in.readByte();
			beginTimestamp = in.readLong();
			closeTimestamp = in.readLong();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public void buildData(DataOutputStream out) {
		// TODO Auto-generated method stub
		try {
			out.writeInt(nodeId);
			out.writeByte(isOpen);
			out.writeByte(isPass);
			out.writeByte(starResult);
			out.writeLong(beginTimestamp);
			out.writeLong(closeTimestamp);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}