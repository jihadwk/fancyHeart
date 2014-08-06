package com.doteyplay.game.config.template;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.doteyplay.game.constants.tollgate.TollgateNodeType;
import com.doteyplay.game.util.excel.TemplateService;

/**
 * @className:TollgateDataManager.java
 * @classDescription:
 * @author:Tom.Zheng
 * @createTime:2014��6��26�� ����1:35:23
 */
public class TollgateDataManager {
	
	
	
	private static TollgateDataManager instance = new TollgateDataManager();
	
	private TollgateDataManager(){}
	
	public static TollgateDataManager getInstance(){
		return instance;
	}
	
	
	public boolean isValidateNodeExsit(int tollgateId,int nodeId){
		return getTollgateData(tollgateId,nodeId)!=null;
	}
	
	/**
	 * ��ȡ�ڵ�ĳ�ʹ���Ǽ�.
	 * @param tollgateId
	 * @param nodeId
	 * @return
	 */
	public final byte getTollgateNodeInitStar(int tollgateId, int nodeId){
		TollgateNodeDataTemplate node =getTollgateData(tollgateId, nodeId);
		if(node!=null){
			if(node.getOpreateType()==TollgateNodeType.ENTER_TOLLGATENODE.ordinal()){
				return TollgateNodeType.ENTER_TOLLGATENODE.getStarInitNum();
			}else{
				return TollgateNodeType.ENTER_BATTLE.getStarInitNum();
			}
		}
		return 0;
	}
	
	/**
	 * ���ؾ����node�ڵ�ԭ������.
	 * @param tollgateId
	 * @param nodeId
	 * @return
	 */
	public final TollgateNodeDataTemplate getTollgateData(int tollgateId, int nodeId) {
		Map<Integer, TollgateDataObject> all = TemplateService.getInstance()
				.getAll(TollgateDataObject.class);

		TollgateDataObject data = all.get(tollgateId);

		if (data == null) {
			return null;
		}

		TollgateNodeDataTemplate node = data.getNode(nodeId);

		if (node == null) {
			return null;
		}
		return node;
	}
	/**
	 * ����ĳһ����,��һ���ڵ�.
	 * @param tollgateId
	 * @param nodeId
	 * @return
	 */
	public final int getFirstNodeId(int tollgateId) {
		
		TollgateNodeDataTemplate node = getFirstNodeData(tollgateId);
		
		if (node == null) {
			throw new RuntimeException("�ؿ��༭����û�нڵ�.tollgateId="+tollgateId);
		}
		return node.getId();
	}
	
	/**
	 * ����ĳһ����,��һ���ڵ�.
	 * @param tollgateId
	 * @param nodeId
	 * @return
	 */
	public final TollgateNodeDataTemplate getFirstNodeData(int tollgateId) {
		Map<Integer, TollgateDataObject> all = TemplateService.getInstance()
				.getAll(TollgateDataObject.class);
		
		TollgateDataObject data = all.get(tollgateId);
		
		if (data == null) {
			return null;
		}
		
		TollgateNodeDataTemplate node = data.getFirstNode();
		
		if (node == null) {
			return null;
		}
		return node;
	}
	
	/**
	 * ������һ�ڵ�.��˳����.
	 * @param tollgateId
	 * @param nodeId
	 * @return
	 */
	public final TollgateNodeDataTemplate getNextNodeData(int tollgateId,int nodeId,Set<Integer> allOpenTollgateIds){
		Map<Integer, TollgateDataObject> all = TemplateService.getInstance()
				.getAll(TollgateDataObject.class);
		
		TollgateDataObject data = all.get(tollgateId);

		if (data == null) {
			return null;
		}
		TollgateNodeDataTemplate node = data.getNextNode(nodeId);
		
		if(node!=null){
			//������Ƿ��Ѿ�����.�������.���ֻ��������Ϣ.���û�п���,������,���Ϳ�����Ϣ.
			return node;
		}
		return null;

	}
	
	public final int getNextTollgate(int tollgateId,Set<Integer> allOpenTollgateIds){
		Map<Integer, TollgateDataObject> all = TemplateService.getInstance()
				.getAll(TollgateDataObject.class);
		//�Ѿ����ŵĹؿ����� == �ܵĹؿ�����.
		if (allOpenTollgateIds.size() == all.keySet().size()) {
			// �����һ����.
			throw new RuntimeException("�ﵽ���һ���ؿ��ڵ���.");
		}
		int nextTollgateId = 0;
		Set<Integer> keySet = all.keySet();
		List<Integer> keyList = new ArrayList<Integer>();
		for (Integer integer : keySet) {
			keyList.add(integer);
		}
		
		Collections.sort(keyList, new Comparator<Integer>(){

			@Override
			public int compare(Integer o1, Integer o2) {
				// TODO Auto-generated method stub
				return o1-o2;
			}
			
		});
		
		for (Integer element : keyList) {
			// �µĹؿ�Id.���Ǳ�����Ĺؿ�Id.
			if(!allOpenTollgateIds.contains(element)){
				nextTollgateId = element;
				break;
			}
		}
		
		return nextTollgateId;

	}
	
	/**
	 * ������ʾ������.
	 * @param tollgateId
	 * @return
	 */
	public int getTollgateShowType(int tollgateId){
		Map<Integer, TollgateDataObject> all = TemplateService.getInstance()
				.getAll(TollgateDataObject.class);
		
		TollgateDataObject tollgateDataObject = all.get(tollgateId);
		
		return tollgateDataObject.getTollgateShowType();
	}

	
	
}