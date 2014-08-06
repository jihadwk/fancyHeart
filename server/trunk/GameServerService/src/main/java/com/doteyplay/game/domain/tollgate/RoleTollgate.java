package com.doteyplay.game.domain.tollgate;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Map;
import java.util.Set;

import org.apache.log4j.Logger;

import com.doteyplay.core.dbcs.DBCS;
import com.doteyplay.game.domain.gamebean.TollgateBean;
import com.doteyplay.game.message.tollgate.ShowTollgateDetailMessage;
import com.doteyplay.game.persistence.serverdata.tollgate.ITollgateBeanDao;

/**
 * @className:RoleTollgate.java
 * @classDescription:������ݿ��Bean
 * @author:Tom.Zheng
 * @createTime:2014��6��23�� ����4:24:13
 */
public class RoleTollgate {

	private static final Logger logger = Logger
			.getLogger(RoleTollgate.class.getName());
	
	private long roleId;
	
	private TollgateBean tollgateBean;
	
	private RoleTollgateManager manager;
	/**
	 * �޲������캯����
	 * @author:Tom.Zheng
	 * @createTime:2014��6��24�� ����3:36:23
	 */
	public RoleTollgate(long roleId) {
		super();
		this.roleId = roleId;
		// TODO Auto-generated constructor stub
	}
	/**
	 * ��ʼ���ռ䡣
	 */
	public void initlize(){
		
		manager = new RoleTollgateManager(roleId);
		
		ITollgateBeanDao roleInstanceBeanDao = DBCS.getExector(ITollgateBeanDao.class);

		tollgateBean = roleInstanceBeanDao.selectTollgateBean(roleId);
		if (tollgateBean == null) {
			tollgateBean = new TollgateBean();
			tollgateBean.setRoleId(roleId);
			addFirstGateData();//���ӵ�һ�أ���һ����㡣��Ϊ��㡣
			buildData();//�������ݣ������⡣
			roleInstanceBeanDao.insertTollgateBean(tollgateBean);
		} else {
			//�Ѿ��е����ݡ�
			readByteInfo();
		}

	}
	/**
	 * ���ݵ���㡣��һ�����������ݿ�����
	 */
	public void addFirstGateData(){
		manager.openTollgateAndNode(100,10001);
	}
	/**
	 * ��ѯ���ݺ󣬽������ݵĲ�����
	 */
	public  void readByteInfo() {
		if (tollgateBean.getTollgateInfo() == null) {
			return;
		}
		DataInputStream in = new DataInputStream(new ByteArrayInputStream(
				tollgateBean.getTollgateInfo()));

		try {
			// ���ݰ汾
			int ver = in.readByte();
			manager.parseData(in, ver);

			in.close();
		} catch (Exception e) {
			logger.error("��" + this.getRoleId() + "���������ݽ�������");
		}
	}
	/**
	 * �������ݣ�����DB֮ǰ���������Ĳ�����
	 */
	public void buildData() {
		
		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(byteArrayOutputStream);

		try {
			// ���ݰ汾
			out.writeByte((byte) 1);

			
			manager.buildData(out);

			out.flush();
			out.close();
			tollgateBean.setTollgateInfo(byteArrayOutputStream.toByteArray());//��ֵ����Ҫ��Ϊ��DB���롣
		} catch (Exception e) {
			logger.error(e.getMessage()+"��" + this.getRoleId() + "���������ݽ�������");
		}
	}
	
	public void addTollGateById(int tollGateId){
//		manager.addFirstTollgateNode(tollGateId, nodeId);
	}

	

	public long getRoleId() {
		return roleId;
	}
	public void setRoleId(long roleId) {
		this.roleId = roleId;
	}
	public TollgateBean getTollgateBean() {
		return tollgateBean;
	}
	public void setTollgateBean(TollgateBean tollgateBean) {
		this.tollgateBean = tollgateBean;
	}
	public void release() {
		// TODO Auto-generated method stub
		if(this.manager!=null){
			this.manager.release();
			this.manager = null;
		}
	}
	/**
	 * ��һ���ؿ����߽ڵ�.ͬʱ����DB
	 * @param tollgateId
	 * @param nodeId
	 */
	public void openTollgateOrNodeAndUpdateDB(int tollgateId, int nodeId) {
		// TODO Auto-generated method stub
		manager.openTollgateAndNode(tollgateId, nodeId);
		updateDB();
	}
	public boolean isOpenTollgateAndNode(int tollgateId, int nodeId) {
		// TODO Auto-generated method stub
		return manager.isOpenTollgateAndNode(tollgateId, nodeId);
	}
	public boolean isOpenTollgate(int tollgateId) {
		// TODO Auto-generated method stub
		return manager.isOpenTollgate(tollgateId);
	}
	public void showTollgateDetailInfo(ShowTollgateDetailMessage message) {
		// TODO Auto-generated method stub
		manager.showTollgateDetailInfo(message);
	}
	public byte acceptBattleResult(int tollgateId, int nodeId, int star) {
		// TODO Auto-generated method stub
		byte isUpdate = manager.acceptBattleResult(tollgateId,nodeId,star);
		 
		if(isUpdate==1){
			updateDB();
		}
		return isUpdate;
	}
	/**
	 * �������ݿ�
	 */
	public void updateDB(){
		ITollgateBeanDao roleInstanceBeanDao = DBCS
				.getExector(ITollgateBeanDao.class);
		buildData();
		roleInstanceBeanDao.updateTollgateBean(tollgateBean);

	}
	/**
	 * �������е��Ѿ����ŵĹؿ�.
	 */
	public Set<Integer> getAllOpenTollgate() {
		// TODO Auto-generated method stub
		
		Map<Integer, TollgateInfo> map = manager.getMap();
		
		return map.keySet();
		
	}

}